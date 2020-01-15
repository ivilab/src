# NO SHEBANG.  This file is sourced, not exec(3)-ed.

# $Id: ties_rf.background.bash 20279 2016-01-25 04:32:40Z predoehl $

# The purpose of this file is to launch background processes.
# * Both must be launched before the start of inference.
# * One (topd) records the output of top(1) to a file every 15 minutes,
#   yielding a history of CPU and memory usage.
# * One (traind) must be launched after doing create_folds.  It will attempt
#   to save the training results, once they are known.  This is useful if the
#   training takes up most of the maximum-allowed runtime,
#   because the testing then can be done later.

###########################################################################
# Launch topd background monitor (kinda redundant with jobstats but oh well).
# * Period set to 900 s (15 minutes) to coincide with getloadavg() output.
(while true
 do (echo; date +%c\ %s; top -b -n 1 | head -30) >>"$log_dir/topd.log"
    sleep 900
 done) &
readonly topd_pid=$!

###########################################################################
# If the user has provided a training tarball, then forget it --
# obviously we do not need to save the training in that case.
# Otherwise wait for all "testing" directories to appear, polling every 3 min.
if [[ -z "${train_filename}" ]] 
then ( testingl=()
       trainl=()
       while read -r -d ''
       do testingl+=("${REPLY%-list.txt}")
          trainl+=("$(dirname "$REPLY")/training")
       done < <(find . -name testing-list.txt -print0)
       while [[ ${#testingl[*]} -gt 0 ]]
       do sleep 180
          for j in ${!testingl[*]}
          do if [[ -d ${testingl[$j]} ]]
             then unset testingl[$j]
             else break
             fi
          done
       done
       for j in "${trainl[@]}"
       do [[ ! -d "$j" ]] && echo >&2 "missing training dir $j" && exit 1
       done
       tar c "${trainl[@]}" | bzip2 >saved_training.tbz
     ) &
     readonly traind_pid=$!
else unset -v traind_pid || DIE
fi

###########################################################################
# This trap causes automatic cleanup for most scenarios (not foolproof).
trap "kill $topd_pid $traind_pid; echo topd, traind killed. >>$bso" 0 1 2 3 15

