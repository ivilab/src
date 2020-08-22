:
:set viminfo=
:set ul=0
:set ttyfast
:
:"Add a blank line, so if there are no files, we do not get an error status 
:"due to 0 lines after we delete the fake_out_glob line.
:1s/.*/
:"
:" The code below assumes that the line is here, and that it is replaced by a"
:" blank line."
:g/fake_out_glob/d
:
:" Change * ==> *.1
:%s/$/.1/ge
:
:" Shift. "
:%s/^/        /e
:
:
:" Add backslashes "
:%s/\([^ ][^ ]*\) */\1 \\/e
:
:
:" Get rid of comments from makedepend "
:%s/^#.*//ge
:
:" Add in PROGRAM_CAT_PATH"
:%s/[A-Za-z0-9_\-]/$(PROGRAM_CAT_PATH)&/e
:
:" Trim all blanks (should be done, but just in case). "
:%s/ *$//ge
:
:
:" Get rid of slash on last item, and add two blank lines. 
:$s/ *\\ */
:"
:1s/.*/
:
:"Add a blank line at the end.
:$s/.*/&
:
:wq
