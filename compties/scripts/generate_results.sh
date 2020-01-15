#!/bin/bash
# Aggregate the errors for the cross-validated results

source $COMPTIES_HOME/scripts/utils.sh

if [[ $# -ne 2  ]]
then
    echo "Usage $0 exp-dir [bin-dir]"
    exit 0
fi

exp_dir=$1
#exp_dir=`pwd $exp_dir`/$exp_dir
bin_dir=$2
echo "Experiment Directory" $exp_dir 
echo "Executable Directory" $bin_dir 

log_dir=$exp_dir/log/
config_fn=$exp_dir/config
fold_dir=$exp_dir/folds
model_dir=$exp_dir/models

num_observ=`grep "observable" $config_fn | wc -l`
load_modules

readonly err_dir="$exp_dir/results"
if [[  -d $err_dir ]]
then
    rm -rf $err_dir 
fi 
mkdir -p $err_dir

BIN=aggregate_fold_errors
test -x "$bin_dir/$BIN" || die "$BIN not executable"
NFSO=`(ldd "$bin_dir/$BIN" | grep "not found")`
test -n "$NFSO" && echo "$NFSO" && die "$BIN cannot be run"
test -d $model_dir || die "$model_dir directory does not exit "

for dir in `ls $model_dir`
do
    res_dir=$model_dir/$dir
    if [[ "$dir" != "average"  && "$dir" != "line" ]]
    then
        runlog $BIN -C "$config_fn" --fold-info-dp $fold_dir -O "$res_dir"
    fi
    # link the two simple baseline models into error directory
    ln -s ${res_dir}/errors "$err_dir/$dir" 
done

# Write a summary of errors
# * The script for this is quite crabbed and complex.  Cases to consider:
#   single observable vs. multiple observable
#   use of grouping variable versus none
#   computing baseline models versus not, and
#   all combinations of the above (because they interact to change the output).
readonly prfn="$exp_dir/primary_results.txt"
: >"$prfn" 
declare -a srcnum errs
# * Utility function to report errors for the average and line models
report_simple_errs() {
    printf >>"$prfn" "\n\n\n\n%d.) RMS errors for %s model (baseline %d):\n\n"\
                     "$1" "$2" "$(($1 - 1))"
    #local e=( "$err_dir"/${2}_*/err_${3}_*.txt )
    local e=( "$err_dir"/${2}/*_err_${3}.txt )
    [[ ${#e[*]} -eq "$num_observ" ]] \
        || DIE "expected $num_observ observables, found ${#e[*]} ${e[*]}"
    errs+=("${e[@]}")
    if [[ "$num_observ" -eq 1 ]]
    then srcnum+=("$1")
         cat >>"$prfn" "${e[0]}"
    else local y z oo
         for ((z=0; z < ${#e[*]}; ++z))
         do y="$1.$((1+z))"
            srcnum+=("$y")
            oo="$(basename "${e[$z]}")"
            oo="${oo%.txt}"
            oo="${oo#err_${3}_}"
            [[ -d "$basedir/$2/$oo" ]] 
            printf >>"$prfn" "\n%s)    For observable %s:\n\n" "$y" "$oo"
            cat >>"$prfn" "${e[$z]}"
         done
    fi
}
# * Utility function to report errors for all cross-validated models
#   $1=heading number, $2=error subdirectory name/glob,
#   $3=long model name plus colon
#   $4 (optional) subheading line 1 (moderators or categorical moderator?),
#   $5 (optional) subheading line 2 (single observables?).
report_crossval_errs() {
    printf >>"$prfn" "\n\n\n\n%d.) RMS errors for %s\n" "$1" "$3"
    [[ $# -ge 4 ]] && printf >>"$prfn" "%s\n" "$4"
    # If $2 is a glob, e1 will enumerate its matches.
    local e1=( "$err_dir"/${2}/err_fold_means.txt )
    local e2=( "$err_dir"/${2}/*_err_fold_means.txt )
    [[ -f "${e1[0]}" ]] || DIE "report cv err 1: ${e1[0]} not found"
    (( ${#e2[*]} == num_observ * ${#e1[*]} )) \
        || DIE "report cv err 2: $num_observ ${e2[*]} for $1 $2 $3"
    if [[ "${#e2[*]}" -eq 1 ]]
    then # single observable and single group
         srcnum+=("$1")
         [[ $# -ge 5 ]] && printf >>"$prfn" "%s\n" "$5"
         echo >>"$prfn"
         cat >>"$prfn" "${e1[0]}"
         errs+=("${e1[0]}")
    else # multiple observables or groups (or both)
         local y z oo
         errs+=("${e2[@]}")
         for ((z=0; z < ${#e2[*]}; ++z))
         do y="$1.$((1+z))"
            srcnum+=("$y")
            oo="$(basename "${e2[$z]%.txt}")"
            oo="${oo#err_fold_means_${model_name}-}"
            printf >>"$prfn" "\n%s)    For observable %s%s:\n\n" \
                             "$y" "$oo" "$(y_gruppo_tambien "${e2[$z]}" "$2")"
            cat >>"$prfn" "${e2[$z]}" || DIE "report cv err 2"
         done
         if [[ "$num_observ" -gt 1 ]]
         then for oo in "${e1[@]}"
              do y="$1.$((++z))"
                 srcnum+=("$y")
                 printf >>"$prfn" \
                     "\n%s)    Average for all observables%s:\n\n" \
                     "$y" "$(y_gruppo_tambien "$oo" "$2")"
                 cat >>"$prfn" "$oo"
                 errs+=("$oo")
              done
         fi
    fi
}

# * Generate auxiliary string of moderating factors for part 1 of $prfn.
use_mgv_str() {
    [[ -n "${moderator}" ]] \
        && printf "    using these moderator(s): %s\n" "${moderator}"
    [[ -n "${categorical_mod}" ]] \
        && printf "    using this categorical moderator: %s\n" \
                  "${categorical_mod}"
}

# * Get the observable 
observable=$(get_observables $config_fn)

# * Start writing the primary results text file
printf >"$prfn" "SUMMARY OF PRIMARY RESULTS\n--------------------------"

# * find the results with moderator 
for dir in `ls $model_dir`
do 
    if [[ "$dir" == *moderator* ]]
    then
        mod_indx=$(strindex $dir "moderator-")
        moderator_str="moderator-"
        let mod_indx=$mod_indx+${#moderator_str}
        moderator=${dir:$mod_indx}
        report_crossval_errs 1 "$dir" \
            "shared-parameter coupled oscillator (the full CompTIES model)" \
            "$(use_mgv_str $moderator)" "    with this observable: $observable"
    fi
done

# * More output if you did the baseline models (which is the common case).
if [[ -d $model_dir/"average" ]]
then 
    report_simple_errs 2 average ave
fi

# line model
if [[ -d $model_dir/"line" ]]
then
    report_simple_errs 3 line line
fi

i=4
for dir in `ls $model_dir`
do 
    if [[ "$dir" != "average"  && "$dir" != "line" && "$dir" != *moderator* ]]
    then
        let index=$i
        report_crossval_errs $index  $dir \ "$dir (baseline $i):"
        let i=$i+1
    fi
done

# * Print filenames of source files for the above tables
declare -p >>"$log_dir/err_summary_debug" srcnum errs
printf >>"$prfn" "\n\n\n\n\nSOURCE(S):\n----------\n"
for ((q = 0; q < ${#srcnum[*]}; ++q))
do printf >>"$prfn" "%s.  %s\n" "${srcnum[$q]}" "${errs[$q]}"
done
unset -v q
