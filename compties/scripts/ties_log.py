#!/usr/bin/env python2.7

# $Id: ties_log.py 19996 2015-10-30 04:14:26Z predoehl $
# Language: python 2.7
#!/usr/local2/predoehl/VIRTUAL/bin/python is the proper shebang on acan
#!/usr/bin/env python2.7 is the proper shebang on most other systems.
# although on TACC you must load the python module first.
#
# This module handles the logging.

# Modules from python's standard library:
import logging
import multiprocessing
import os, os.path
import subprocess
import sys



# multiprocessing lock, used to synchronize access to the log file
mp_log_lock = None


def _quotify(s):
    '''
    Convert string s into a string that prints out as a valid python string
    literal.  The literal is triple-quoted, and possibly contains injected
    backslash escapes, if unavoidable.  Example:  quotify('cat') is '"""cat"""'
    i.e., input is three characters long, output is nine characters long.
    If the input has one kind of triple-quote, we use the other kind.  If it
    has both kinds, we inject backslash escapes to make it a valid literal.
    '''
    sl = '"""' + s + '"""' # this is enough for the common case
    if '"""' in s: # detect uncommon cases
        if "'''" in s:
            sl = '"""' + s.replace('"""', '\\"""') + '"""'
        else:
            sl = "'''" + s + "'''"
    return sl


def log_info_output(output01, prog):
    'Write subprocess (pipe) output to log file as info, synched with MP lock.'
    assert mp_log_lock is not None
    mp_log_lock.acquire()
    try:
        for ostr, kind in zip(output01, ['Standard output', 'Standard error']):
            if len(ostr) > 0:
                logging.info(kind +' of '+ str(prog) + ':\n' + _quotify(ostr))
    finally:
        mp_log_lock.release()


def _log_host_capabilities():
    'Spy on computing host just a little bit; write answers to info log.'
    for prog in ['hostname','date','/sbin/ifconfig',['cat','/proc/cpuinfo']]:
        output = subprocess.Popen(prog,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        log_info_output(output, prog)


def failsafe_mkdir(pathname):
    '''
    This will create a directory, if the pathname does not previously exist.
    If it already exists, this function has no effect (unlike os.mkdir).

    It is used for the directory that stores the log file.  This is an example
    of what in Python is called an "EAFP approach" -- easier to ask forgiveness
    than permission.  We always try to create the directory, and swallow the
    exception that occurs whenever the directory already exists.
    EAFP is in contrast to the so-called LBYL philosophy -- look before you
    leap.  Although LBYL might at first feel more correct, if done correctly it
    is very complicated, and if done incorrectly leads to a race condition.
    EAFP avoids a race condition and is very simple, though I feel compelled
    to write this wordy justification.

    This was formerly named with a leading underscore to make it "private,"
    but it has proved useful elsewhere so I'm renaming it to be "public."
    '''
    try:
        os.makedirs(pathname) # Note:  if it already exists, this will throw.
    except os.error:
        pass # "If it already exists, just forget I asked." (EAFP philosophy)


def _top_sleep_repeat():
    '''
    Shadow process that mostly sleeps, occasionally calls top(1) and logs it

    Don't use check_output() here because it's a Python 2.7 feature and we
    are forced to restrict ourselves to 2.6 features on DE. :-(
    '''
    while True:
        t = subprocess.Popen('sleep 3600; date; top -b -n 1 | head -25',
         shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
        assert mp_log_lock is not None
        mp_log_lock.acquire()
        try:
            logging.info('Shadow status, ' + t[0])
            if len(t[1]) > 0:
                logging.debug('Shadow status error: ' + t[1])
        finally:
            mp_log_lock.release()


def start_log_with_pathname(log_pathname):
    '''
    Turn on debug-logging for this script.
    We used to fork a process to log status.  Now we don't,
    but that machinery still exists, it just isn't used.
    Return value is now None.  Formerly it was the Process object.
    '''
    global mp_log_lock
    assert mp_log_lock is None
    mp_log_lock = multiprocessing.Lock()
    mp_log_lock.acquire()
    log_dirname = os.path.dirname(log_pathname)
    try:
        failsafe_mkdir(log_dirname)
        logging.basicConfig(filename=log_pathname, level=logging.DEBUG)
        logging.debug('Invocation: ' + str(sys.argv))
    finally:
        mp_log_lock.release()
    _log_host_capabilities()
    #pid = multiprocessing.Process(target=_top_sleep_repeat)
    #pid.start()
    #return pid


def start_log_with_args():
    '''
    Start a log file, using optional command-line args if they are present,
    otherwise with a reasonable default filename.

    The default is to write to directory "logs," in a similarly-named file with
    .log suffix.  Because that default causes problems in a multiprocessing
    environment, we allow some homebrew options, not handled by
    ties_options.py, to specify a different log file.  They are
    --log-dirname DIR --log-basename LOGFN

    As the flags suggest, these control the log file directory name and
    filename.  Behavior is equivalent to DIR having a default of 'logs'
    and LOGFN having a default of the filename of the script, with a '.log'
    suffix instead of the '.py' suffix (which is expected to be present).
    If you use these options, use either or both just as shown:  do not try
    to use equals-syntax.  Note these options won't appear in the log;
    they are consumed beforehand.  Sorry that seems so hacky.
    '''
    argc = len(sys.argv)
    log_dirname = 'logs'
    if '--log-dirname' in sys.argv:
        ldi = sys.argv.index('--log-dirname')
        assert ldi > 0, 'zero-index arg is the name of the script, not this'
        if ldi + 1 < argc:
            log_dirname = sys.argv[ldi + 1]
            del sys.argv[ldi : ldi+2]
            argc -= 2
            assert len(sys.argv) == argc, 'yes we know how many args there are'

    assert sys.argv[0].endswith('.py'), 'Name of script ends in dot pea why'
    log_basename = os.path.basename(sys.argv[0])[:-3] + '.log'
    if '--log-basename' in sys.argv:
        lbi = sys.argv.index('--log-basename')
        assert lbi > 0, 'zero-index arg is not this flag either'
        if lbi + 1 < argc:
            log_basename = sys.argv[lbi + 1]
            del sys.argv[lbi : lbi+2]

    return start_log_with_pathname(os.path.join(log_dirname, log_basename))


def log_debug(s):
    'Write a debug string to the debug log, synched with MP lock.'
    assert mp_log_lock is not None
    mp_log_lock.acquire()
    try:
        logging.debug(s)
    finally:
        mp_log_lock.release()


def stop_hourly_top_log(log_process):
    if log_process is None:
        mp_log_lock = None
        return
    assert mp_log_lock is not None
    mp_log_lock.acquire()
    try:
        logging.debug('Now, stop_hourly_top_log().')
        log_process.terminate()
    finally:
        mp_log_lock.release()
    mp_log_lock = None


def log_warning(msg):
    'Write a warning string to the log, synched with MP lock.'
    assert mp_log_lock is not None
    mp_log_lock.acquire()
    try:
        logging.warning(msg)
    finally:
        mp_log_lock.release()



if __name__ == '__main__':
    pid = start_log_with_pathname('ties_log_demo.log')
    try:
        log_debug('This is a sample debug message.')
        log_warning('THIS IS A WARNING, but just a sample.')
    finally:
        stop_hourly_top_log(pid)

