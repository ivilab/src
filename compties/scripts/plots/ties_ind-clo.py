#!/usr/bin/env python2.7

# $Id: ties_plots.py 20833 2016-09-01 20:20:30Z jguan1 $
# Language: python 2.7
#!/usr/local2/predoehl/VIRTUAL/bin/python is the proper shebang on acan
#!/usr/bin/env python2.7 is the proper shebang on most other systems.
# although on TACC you must load the python module first.
#
# This module does the plots for the Python wrapper on TIES.  It is TOTALLY
# coupled to the directory structure and file format of the TIES output.

# This script is used three ways:  as a standalone script, and as a module
# for ties_basic_complete_run.py and ties_singlefold_run.py.
# As a module, its interface is defined by
#
# 1)  Function generate_individual_plots(args), where args is the Namespace
#     of options used to invoke the program.  See that function for a list
#     of which options are examined.  The field names are self-explanatory.
# 2)  File system of appropriate state.  This script read from and writes to
#     the file system, via paths named in args.  Those paths are required to
#     have a subdirectory topology of correct shape, with input files in the
#     right place, and with the expected format.  and this writes output files
#     to paths in and under args.output_dir_plots.  The paths are implicitly
#     specified by the glob constructors in the code.  The file formats are
#     undocumented; they were inferred from examples by the developer of this
#     script (Predoehl).
#
# Output "plots" are saved in three formats:  PNG images, encapsulated
# postscript, and comma-separated variable text.  Output filenames are built
# from dyad numbers, model name (for baselines only), and output format.
# Into the pathname specified in 'output_dir_plots' this writes the
# time-series plots for the full TIES model.  If baseline models are used,
# the baseline plots are written into a subdirectory called 'baselines.'
#
# This script is used as a module of ties_basic_complete_run.py and also by
# script ties_singlefold_run.py.  Furthermore this script can run stand-alone,
# in the event that the inference results have already been computed.  This
# use case is a bit awkward, since the script options are handled by the same
# module used to launch inference (ties_options.py).  That module demands
# certain --flag VALUE arguments that do not make logical sense to the third
# use case (e.g., --binary-baseline-engine VALUE).  Unfortunately the user
# must just endure the awkwardness, and supply the extra arguments, knowing
# that they won't actually influence the plot output, provided they pass the
# validation tests.  See tacc/parallel_complete_ties.sh for example invocation.
#
# Plots are created using Matplotlib.  Missing observable values are
# tolerated, and these are plotted by creating a break in the curve,
# implemented using numpy masked arrays.  The fitting/prediction boundary is
# shown using a vertical half-plane as specified by prediction_fraction.
# The legend (a text box at right) can be switched on or off via the
# USE_LEGEND global.  It might be helpful now to summarize the abstractions
# used by Matplotlib (in general, not just here); future maintainers might
# benefit by carefully noting the following terminology, prior to jumping into
# Matplotlib documentation:
# * Input files are parsed and then PLOTTED onto a FIGURE.
# * A figure is RENDERED onto a CANVAS.
# * A canvas is EXPORTED to file formats PNG and EPS.
#
# Parsing:  _get_dyad_observables, _gen_model_plts
# Plotting: _draw_basic_plot, _plot_without_legend, _plot_with_legend
# Rendering, exporting:  _gen_a_model_plot 
# (CSV output is not via Matplotlib; we call _write_csv with parsed data.)


import csv
import glob
import matplotlib.figure, matplotlib.backends.backend_agg
import numpy, numpy.ma
import os, os.path
import re
import argparse

# Custom modules:
import ties_log
import ties_options


# Do you want the individual plots to show a legend describing the curves?
USE_LEGEND = False

# Description of the full-power CompTIES coupled oscillator model,
# rendered in filename-friendly format (as a sentinel value).
# It isn't actually used in a filename, but this string is used as the column
# label in the CSV output, which I believe should look filename friendly also.
IMPLICIT_COMPTIES_MODELNAME = 'shared-param-CLO-with-moderator'

# Look for a directory in path that looks like /0023/ or similar:
dynum_regex = re.compile(r'/(\d{4})/')


# Special string that symbolizes missing data in the observables files.
MISSING_SENTINEL = None


def _write_csv(csv_fname, onam, model_name, data, model):
    'Write this plot (for one dyad, one model) in CSV text format to a file.'
    f = open(csv_fname, 'w')
    wr = csv.writer(f, quoting=csv.QUOTE_NONNUMERIC)
    wr.writerow(['time', onam+'-0', onam+'-1',
                 model_name+'-0', model_name+'-1'])
    for t in sorted(data.keys()):
         row = [t]
         row.extend(data[t])
         row.extend(model[t])
         wr.writerow(row)


def _draw_basic_plot(ax, title, onam, tn, o0, o1, m0, m1, prediction_fraction):
    '''
    Plot the four curves on the plot-axes, and label the axes.

    Also this places a colored highlight strip on the prediction time range.
    This function "knows" the prediction time range comes at the end of
    the time data, and prediction_fraction must be a value in interval (0,1)
    indicating how much time is set aside for prediction.  This value has
    been 0.2 for a long time but I don't want to hard-code it here.

    Return value is a tuple of the handles of the four plotted curves.
    Handle tuple order is same as the inputs:  handles for o0, o1, m0, m1.
    Those params are expected to correspond to observables for person 0 and 1,
    and model state for person 0 and 1, respectively.

    Values t000, t100 represent the limits of the horizontal (time) display,
    whereas min(tn) and max(tn) represent the limits of the time range of data.
    Often, t100 exceeds max(tn), since the plotting code rounds up its x (and
    y) range to limits that are visually clearer and neater.
    The start of the prediction timerange depends on tn's min and max, and on
    prediction_fraction, but the right edge of the highlight strip,
    for neatness's sake, is set to t100.
    We have to turn off autoscaling before adding the highlight strip,
    otherwise the axes sometimes rescale themselves to engulf t100 better.
    '''
    handles = ax.plot(tn, o0, 'r', tn, o1, 'b', tn, m0, 'r--', tn, m1, 'b--')
    ax.set_title(title)
    ax.set_xlabel('Time step')
    ax.set_ylabel('Response (' + onam + ')')
    assert  0 <= prediction_fraction <= 1, 'fit using 0-100% of time range'

    ax.autoscale(False) # turn off autoscaling before adding yellow highlight
    t000, t100 = ax.get_xlim() # t100 := rounded_up_somehow(max(tn)).
    fit_fraction = 1 - prediction_fraction
    t080 = prediction_fraction * numpy.min(tn) + fit_fraction * numpy.max(tn)
    ax.axvspan(t080, t100, facecolor='yellow', alpha=0.2)
    return handles


def _plot_without_legend(title, d, onam, tn, obs0, obs1, model0, model1):
    'Generate and return a figure, on which we draw the basic plot.'
    fig = matplotlib.figure.Figure()
    ax = fig.add_subplot(111)
    _draw_basic_plot(ax, title, onam, tn, obs0, obs1, model0, model1,
        prediction_fraction=0.2)
    return fig


def _plot_with_legend(title, distinguisher, onam, tn, obs0, obs1, model0, model1):
    '''
    Generate and return a figure, on which we draw the basic plot and legend.
    This function controls how much to pull back the axes to make room for the
    legend.  If you want a smaller legend, go to work here.
    The add_axes method used normalized coords in domain {x,y : 0 <= x,y <= 1}.
    The four parameters indicate the axes x-position, y-pos, width, height.
    '''
    fig = matplotlib.figure.Figure()
    ax = fig.add_axes([0.1, 0.1, 0.58, 0.8])
    h = _draw_basic_plot(ax, title, onam, tn, obs0, obs1,
        model0, model1, prediction_fraction=0.2)
    d0 = distinguisher + '-0'
    d1 = distinguisher + '-1'
    fig.legend(h,
        (d0+' data', d1+' data', d0+' model', d1+' model'),
        loc='center right')
    return fig



def _gen_a_model_plot(plot_path_stem, onam, dnam, dyad_num, model_name, data, model):
    '''
    Inputs correspond to one dyad, one flavor of model, one observable cateory,
    and we want to plot the timeseries of both, overlaid together.  The plot
    is saved to files, with path and filename glued together from the inputs,
    and in a few formats (currently PNG, EPS, and CSV).

    String 'onam' is the name of the observable being plotted.
    String 'dnam' is the name of the distinguisher between the dyad individuals.

    String 'plot_path_stem' is part of the pathname of the output file -- it
    still lacks a filetype extension.

    String 'dyad_num' is the zero-padded decimal string of the dyad number.

    String 'model_name' describes the model, and it gets used in the CSV
    output.  Also it is altered slightly and used in the plot title, except
    when equal to sentinel value IMPLICIT_COMPTIES_MODELNAME, which triggers
    the title to use a slightly prettier string with the same meaning.
    Basically, it should either equal IMPLICIT_COMPTIES_MODELNAME or be a
    baseline model description.

    Empirical observations are stored in dictionary 'data' and the inferred
    explanatory state is stored in dictionary 'model' -- both indexed by the
    same set of keys (time indices), each yielding a 2-tuple for a valid time
    index.  Tuple members are usually floats, sometimes None if a datum is
    missing.

    Contents of 'data' and 'model' are translated to numpy masked arrays.
    In this function each None entry (if any) is translated into a special
    numpy value 'numpy.ma.masked' that appears in the plots as a gap in
    the curve, i.e., it looks totally appropriate in my judgment.
    '''
    nonemask = lambda x: x if x is not None else numpy.ma.masked
    ti = sorted(data.keys()) # time as array of native Python ints
    # Next assertion was checked earlier already, and is a bit slow.
    # So let's not do it twice, at least not in production code.
    #assert ti == sorted(model.keys()), 'same time domain in data, model'
    obs0 = numpy.ma.array([nonemask(data[t][0]) for t in ti])
    obs1 = numpy.ma.array([nonemask(data[t][1]) for t in ti])
    model0 = numpy.ma.array([nonemask(model[t][0]) for t in ti])
    model1 = numpy.ma.array([nonemask(model[t][1]) for t in ti])

    title = 'Dyad ' + dyad_num + ' observable data and model fit,\n'
    if IMPLICIT_COMPTIES_MODELNAME == model_name:
        # use a slightly less-terse, more readable equivalent phrase.
        title += 'Shared-parameter moderated coupled oscillator model'
    else:
        title += model_name.capitalize() + ' baseline model'

    # Generate output:  plot curves, render plot, export renderings.
    # Also save a CSV of underlying data.
    plot_fn = _plot_with_legend if USE_LEGEND else _plot_without_legend
    fig = plot_fn(title, dnam, onam, numpy.array(ti), obs0, obs1, model0, model1)
    canvas = matplotlib.backends.backend_agg.FigureCanvasAgg(fig)
    canvas.print_figure(filename=plot_path_stem + '.eps')
    canvas.print_figure(filename=plot_path_stem + '.png')
    _write_csv(plot_path_stem + '.csv', onam, model_name, data, model)


def _get_categories(label_list):
    '''
    The input is a list of strings, hopefully that looks like
    ['old-0', 'old-1', 'new-0', 'new-1', 'brwd-0', 'brwd-1', 'blue-0','blue-1']
    i.e., category names repeated in pairs with alternating -0 and -1
    suffixes.  List length must be even, size 2 or larger.
    This returns the category names, in order they were found, for example,
    ['old', 'new', 'brwd', 'blue'].
    If the format is off this raises an exception.
    '''
    if len(label_list) < 2 or len(label_list) % 2:
        raise Exception('Label list bad length ' + str(len(label_list)))
    c = []
    for i, l in enumerate(label_list):
        if len(l) < 3:
            raise Exception('Bad label '+l+' in label list')
        s = l[:-2]
        if l[-2] == '_':
            ties_log.log_debug('Warning: label line has underscores '
                               + 'not dashes')
            l = l[:-3] + '-' + l[-1] # i.e., l[-2]='-' but Python disallows it.
        if i % 2:
            if s != c[-1] or l[-2:] != '-1':
                raise Exception('Failed alternation 1 in label line')
        else:
            if l[-2:] != '-0':
                raise Exception('Failed alternation 0 in label line')
            c.append(s)
    return c


def _get_fname(dyadnum, num_obs, obs_cat_name, modelname, plotdir):
    '''
    This returns a string equal to the stem of the output path for the
    plot.  It still lacks any file extension, i.e., no ".png" is attached.
    The logic here is rather fussy, and depends on wanting to make filenames
    that are sufficiently clear but not too long, and filesystem friendly.
    '''
    # * Filename should always say the dyad number.
    fnam = 'dyad_' + dyadnum
    # * Filename also should say observable IF there are multiple.
    if num_obs > 1:
        fnam = obs_cat_name + '_' + fnam
    # * Filename should say model if it's not implicitly understood.
    # * Also we apply some hand-curated abbreviations. ;-)
    if modelname != IMPLICIT_COMPTIES_MODELNAME:
        m2 = modelname.replace(' ','_').replace(',','').replace('.','')
        m2 = m2.replace('moderator','mod').replace('oscillator','osc')
        fnam += '_' + m2.replace('coupled_osc','CLO')
    fnam = os.path.join(plotdir, fnam) # prepend destination path
    # fnam still lacks an extension though.

    return fnam


def _gen_model_plts(modelname, dnam, plotdir, data, modelfns):
    '''
    Inputs correspond to all dyads, one flavor of model (maybe a baseline,
    maybe the TIES model).  Output is a series of plots which are written
    into the plot directory named in 'plotdir.'

    Input 'modelname' describes the model, either (1) in a human-readable form
    (i.e., using spaces, suitable for a plot title, and not suitable for a
    filename), in case of a baseline model; or (2) sentinel value
    IMPLICIT_COMPTIES_MODELNAME if it is the main CompTIES model result.

    Input 'dnam' is the string name of the distinguisher category.

    Input 'data' is a multi-level dictionary of all dyads' observable data.
    See _get_dyad_observables() for the format (because 'data' is the output
    of that function).

    Input 'modelfns' is a list of filenames of all dyads' model state.  The
    dyad number is extracted from the filename; this part is a bit arcane.
    The file format is basically columnar text delimited by whitespace.
    The array is rectangular.  The first line has labels made of the observable
    names with a person suffix (e.g., respiration-0 respiration-1 bp-0 bp-1).
    Sadly the columns label order does not necessarily align with the order
    in the data files, which is why 'data' has a somewhat arcane structure.
    We have to parse the first line to determine what the columns mean.
    '''
    ties_log.log_debug('_gen_model_plts for modelname=' + modelname
        + ', plotdir=' + plotdir + ', modelfns=' + repr(modelfns)
        + ', len(data) is ' + str(len(data)) + '\n')
    '''for o in data.keys():
        if len(modelfns) != len(data[o]):
            raise Exception('num. dyads of data and model differ')'''
    ties_log.failsafe_mkdir(plotdir)
    for m in modelfns:
        # Extract dyad number from filename
        dyma = dynum_regex.search(m)
        if dyma is None:
            raise Exception('model state path ' + m + ' lacks dyad number')
        dyadnum = dyma.group(1) # dyad number of this model file . . .

        # Get starting time index
        t0 = None
        for ov in data.itervalues():
            if t0 is None:
                t0 = min(ov[int(dyadnum)].keys()) # first time index
            elif min(ov[int(dyadnum)].keys()) != t0:
                raise Exception('Ragged data: file ' + m + ' dyad ' + dyadnum
                                + ' has irregular start time')
        assert t0 is not None, 'found starting time index t0 already'

        # Read the whole file (barely parsed, as rectangular array of strings)
        with open(m, 'r') as model_file:
            ll = model_file.readline().split() # First line is the label line.
            mlines = [[float(x) for x in ml.split()] for ml in model_file]

        # Given the file's column-pair number, which observ. name corresponds?
        cio = { i : o for i, o in enumerate(_get_categories(ll)) }
        if sorted(cio.values()) != sorted(data.keys()):
            raise Exception('Model file, data file have different observables')

        # Build dictionary of model-state data in the file, i.e., parse harder.
        pd = {oc:{} for oc in data.iterkeys()} # first index is observable name
        for t, ms in enumerate(mlines): # parse each line
            t += t0
            for j, s in enumerate(ms): # parse each column
                oc = cio[j/2] # observable category name for column j
                if t not in pd[oc]:
                    assert 0 == j%2, 'start of tuple is for person 0'
                    pd[oc][t] = (s, None)
                else:
                    assert 1 == j%2, '2nd half of tuple is for person 1'
                    s2 = pd[oc][t]
                    assert 2==len(s2) and s2[1] is None, 'halfway done'
                    pd[oc][t] = (s2[0], s)

        # Generate one plot per observable category found in this file
        for oc in data.iterkeys():
            d = data[oc][int(dyadnum)] # data for this obsv category and dyad
            if len(d.keys()) != len(pd[oc].keys()):
                raise Exception('Model state time interval differs from data')
            assert sorted(d.keys()) == sorted(pd[oc].keys()), 'times match'

            # Generate plot filename and plot file
            fname = _get_fname(dyadnum, len(cio), oc, modelname, plotdir)
            _gen_a_model_plot(fname, oc, dnam, dyadnum, modelname, d, pd[oc])


def _get_dyad_observables(fname_list):
    '''
    Input: list of pathnames to files, each storing a dyad's observable data.
    Dyad number is encoded in the pathname.  File format is set by C++ code.

    Output:  a multi-level dictionary.  First index:  observable category name
    (like "dial" or "respiration" but no individual specifier:  not "dial-1").
    Second index: dyad number (as integer).  Third index: time step (integer).
    That yields a 2-tuple of floats, corr. to individual 0 and 1 respectively,
    with an exception for missing data.  If any data are missing, there's None
    in the 2-tuple instead of a float (in either or both positions).

    File format has a prologue before the data.  The prologue, which we skip,
    ends with a line that looks like "time  dial-0  dial-1" (example)
    in the case that the observable category is called "dial."  The observable
    can be called anything, though.  Anyway, we look for a line (the last,
    actually) with "time" as the first whitespace-separated token on the line,
    and assume it is the last line (the "time-line") of the prologue.
    '''
    all_obs = {} # index by observable category name, then dyad number,
                 # then time, then individual.
    ties_log.log_debug('call get_dyad_observables() with ' + repr(fname_list))
    for fname in fname_list:
        assert fname.endswith('.txt'), 'last four chars describe file-type'
        dyadnum = int(os.path.basename(fname)[:-4]) # dyad number of input data

        # Read file, scan for heading line and observable names.
        with open(fname, 'r') as fd:
            flines = [dl.split() for dl in fd]
        timeline = max([n for n, a in enumerate(flines) if 'time' == a[0]])
        obsline =max([n for n, a in enumerate(flines) if 'observables:'==a[0]])
        obsv_names = flines[obsline][1:]

        # Initialize dictionary if necessary, otherwise verify obsv_names.
        if 0 == len(all_obs):
            all_obs = { o : {} for o in obsv_names }
        elif sorted(all_obs.keys()) != sorted(obsv_names):
            raise Exception('Observables disagree across data files')
        arity = len(flines[timeline]) - 1 # number of data columns
        if arity % 2 != 0 or len(obsv_names) != arity/2:
            raise Exception('bad arity parity')

        # Verify heading names, like obsv_names with '-0' and '-1' interleaved.
        ohead = ' '.join([(o+'-0 '+ o+'-1') for o in obsv_names]).split()
        if flines[timeline][1:] != ohead:
            raise Exception('Observable line and timeline disagree')

        # Add blank dictionary of time:(None,None) for this obsv. and dyad.
        for on in obsv_names:
            all_obs[on][dyadnum] \
                = {int(l[0]) : (None, None) for l in flines[timeline+1:]}
        cio = { i : on for i, on in enumerate(obsv_names) }

        # Read each time-observables line (a list of strings of numbers).
        for toblin in flines[timeline+1:]:
            if len(toblin) != 1+arity:
                raise Exception('wrong len data line '+ fname +', '+ toblin[0])
            t = int(toblin[0]) # time index
            for oi, ov in enumerate(toblin[1:]):
                on = cio[oi/2] # observable name of category (e.g., 'dial')
                dd = all_obs[on][dyadnum][t] # get prior value, to augment
                # Missing values in input are marked with a sentinel value.
                ov = float(ov) if ov != MISSING_SENTINEL else None
                if oi % 2:
                    all_obs[on][dyadnum][t] = (dd[0], ov)
                else:
                    all_obs[on][dyadnum][t] = (ov, dd[1])
    return all_obs


def _get_missing_sentinel(data_dir):
    '''
    Read the sentinel value for missing data, which parse_real_data writes
    into file "invalid.txt" wherever the data lives.  We use a regex to
    chomp off any newlines or whitespace.
    '''
    print data_dir
    with open(os.path.join(data_dir, 'invalid.txt'), 'r') as f:
        s = f.read()
    g = re.search(r'\S+', s) # grab the first block of non-whitespace
    if g is None:
        raise Exception('Blank invalid.txt file')
    return g.group(0)


def generate_individual_plots(args):
    '''
    Generate all output plots of individuals of all dyads.
    This function "knows" about the pathnames to the baseline model fits,
    the TIES model fit, and the observable input data.
    Its helper functions "know" about the format of those files.
    If the TIES code changes that format, this function and its helpers will
    need retouching.
    Outputs are exported files written to the plots subdirectory.  That subdir
    is created if necessary, as named in output_dir_plots.  The baseline plots
    (if any) are written into a directory as named in output_dir_baselines.

    The arguments called 'output_dir_model' and 'output_dir_baselines' are
    confusing to a casual reader of this script; really they are inputs.
    They are so-named because those names come from ties_options.py, and are
    based on the point of view of the high-level caller.  From the point of
    view of this script, they are read-only.

    The 'moderator' value is only meaningful in a binary sense, that is,
    whether its value is None or not.  If None, that means that no
    --moderator flag was provided on the command line, and it means that
    only baseline models are being computed.  Otherwise, the value is
    ignored.

    The logic in this function is crabbed and sclerotic.  Needs overhaul.

    Required names of 'args' namespace:
    * output_dir_baselines (misnomer, see above)
    * output_dir_model (misnomer, see above)
    * output_dir_plots
    * moderator  (see above)
    * categorical_mod 
    * skip_baseline_models
    * skip_shared_param_baseline
    * data_dir

    Maintenance note:  older versions of this function had one weird trick
    to work around a Condor bug, but the trick has been removed.  The bug was
    that Condor would misguidedly prune any output subdirectory that lacked
    any regular files.  That might sound harmless but entails pruning an
    output subdirectory containing other subdirectories -- thus throwing away
    their contents.  The bug was reported to iPlant but I was never informed
    of a fix, so I added a workaround for a partial-results situation, by
    adding a token regular file, and later removing it if I got full results
    (not just partial results).  Due to the fact that we don't use Condor
    anymore for CompTIES and that I reorganized the way output directories
    get created, this code became a liability and hence was removed.
    But if you return to Condor, the technique might be helpful,
    and that's why I'm memorializing the technique in this comment
    even though it is now vanished from the code.
    '''
    glob4d = '[0-9][0-9][0-9][0-9]' # used for dyad identification number
    fold_states = os.path.join('fold-[0-9][0-9]', 'testing', glob4d,
                               'all_states', 'obs_states.txt')
    baseline_dir = args.output_dir_baselines
    data_dir = args.data_dir

    print data_dir
    global MISSING_SENTINEL
    MISSING_SENTINEL = _get_missing_sentinel(args.data_dir)
    '''ties_log.log_debug('invalid text string: ' + str(MISSING_SENTINEL))
    '''
    dflist = glob.glob(os.path.join(args.data_dir, glob4d + '.txt'))
    mslist = None
    '''ties_log.log_debug('generate individual plots() globs:\ndflist:\n'
        + repr(dflist) + '\nmslist_c0:\n' + repr(mslist))
    '''
    print dflist
    dyad_obs = _get_dyad_observables(dflist)

    if 0 == len(dyad_obs):
        raise Exception('Must be at least one dyad in dataset')

    kfold_sorter = lambda fn: fn[fn.find('testing'):]

    _gen_model_plts('ind-clo', args.distinguisher,
        os.path.join(args.output_dir_plots, 'baseline-2-line'),
        dyad_obs, sorted(glob.glob(os.path.join(
            baseline_dir, glob4d,'all_states', 'obs_states.txt'))))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Plot the posteriors')
    parser.add_argument('--output-dir-baselines', 
        help='Model directory(Required)')
    parser.add_argument('--distinguisher',
        help='Model directory(Required)')
    parser.add_argument('--output-dir-plots',
        help='Model directory(Required)')
    parser.add_argument('--data-dir', 
        help='Model directory(Required)')
    args = parser.parse_args()
    generate_individual_plots(args)
