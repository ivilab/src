#!/bin/bash

# Stub version of archive function that will be overridden later.
archive_workdir() { echo "(Cannot archive now)" | tee -a "$bso"; }
tattle() {
    printf "\n\n\n**** BEGIN ERROR FILE %s:\n\n%s\n" "$1" "$2"
    printf "*****  END ERROR FILE %s\n\n\n" "$1"
}

die() {
    if [[ "$log_dir" != "" ]]
    then
        local bso="$log_dir/job_run_status"
        printf >>"$bso" "**** app error: %s\n" "$*"
        local x f="$exp_dir/FAILURE_REPORT.txt"
        for x in *.err
        do 
            if grep 1>/dev/null 2>/dev/null "CANCEL" "$x"
                then tattle >>"$f" "$x" "$(sed -n "/CANCEL/p" <"$x")"
            fi
        done
        for x in "$log_dir"/*.stderr "$bso"
        do 
            if [[ -s "$x" ]]
            then tattle >>"$f" "$x" "$(< "$x")"
            fi
        done
        # IMPORTANT: In case dry run fails, DO NOT PROVIDE CONFIG
    	#rm -rf $config_fn 
        
        exit 1
    else
        printf >/dev/stderr "$0 error: %s\n" "$1"; exit 1;
    fi
}

readonly -f tattle die
DIE() { die "Deployment failure (this function never should run). $*"; }

#die () { printf >/dev/stderr "$BN0 error: %s\n" "$1"; exit 1; }
# Filter standard input, char by char, passing alphanumeric or any of "_,-."
sanitize () { printf %s "$1" | tr -d " ;&|(){}<>\n\t"; }
sanit() { tr -cd "A-Za-z0-9_,.-"; }
print_help () {
cat <<EOF
NAME
    $BN0
    Run TIES model using one process per fold.

SYNOPSIS
    --help                      Prints this help message and exits.

The following argument pairs below are mandatory.

    --data-csv-file     Name of CSV file containing experimental data.

    --exp-dir           Name of the experiment folder.

    --bindir	        This is a directory that contains all the exectuables.

The following options must be specified as argument pairs or using the config file.

    --observable        Category name of observable data. Multiple observable 
                        values are specified as seperate flag value pairs.
       
    --distinguisher     Category name of the 0-1 category distinguishing the 
                        individuals within the dyad.

    NOTE: We have to specify atleast one of either --run-base-lines or --moderators


The arguments specified below are optional


    --config-file       Name of file with configuration options.
                        If the user provides a config file, the user config 
                        options will be appended to the default template.

    --run-base-lines    If specified, then baseline models will be executed.

    --moderator         Name of a moderator factor. Multiple moderator values are specified
                        as seperate flag value pairs.

    --categorical-moderator   Category name of categorical moderator.

    --model-offset      This corresponds to "--polynomial-degree=0" option in the C++ program.

    --model-linear-trend This corresponds to "--polynomial-degree=1" option in the C++ program.

    --mean-centered     This corresponds to "--mean-centered" option in the C++ program.

    --average-size      This corresponds to "--average-size" option in the
                        C++ program (which allows to average the observables 
                        across multiple time points)

    --allow-drift       This corresponds to "--allow-drift" in the C++ program

    --fit-all-data      This corresponds to "--fit-all-data" in the C++ program

    --queue-name        This corresponds to the queue name (on elgato, it
                        could be windfall or medium).

    --input-dir         This is a directory that contains the trained models.
    
    --dry-run		    If specified, run with 0 iterations.

    --exclude-percent   If specified, excludes the last specified percent of 
                        time points from the data file.	

     --time-interval-seconds  Time interval between data points in the data csv file.
EOF
}

parse_options()
{
# No of arguments has to be 1 or >5
# If it is one then it has to be --help. 
if [ $# -eq 1 ]; 
then
    FLAG=$(sanitize "$1") || die "cannot read flag $1"
    if [ "$FLAG" = "--help" ];
    then 
        print_help && exit 0
    else
        die "Bad syntax: arg $FLAG not in a --flag=VALUE pair"
    fi
else
    # If $#>1,then atleast 2 required --flag=VALUE pairs must be present
    if [ $# -lt 3 ]; 
    then
        die "Bad Syntax: Input file, bin and experiment folder required. Refer $0 --help"
    fi

    while [ $# -gt 0 ]
    do  
        OP=$(sanitize "$1") || die "cannot read flag $1"
        shift
        # Split the  string using = as delimiter.
        IFS=’=’ read -ra FLAGVAL <<< "$OP" 
     
        FLAG=${FLAGVAL[0]}
        VALUE=${FLAGVAL[1]}      

        case "$FLAG" in
   	        --exp-dir) 
                exp_dir="$VALUE" 
                ;;
       	    --data-csv-file) 
                data_file="$VALUE" 
                ;;
            --config-file) 
                config_file="$VALUE"
                ;;
            --observable) 
                OBS="$OBS $FLAG $VALUE"       
                ;;
            --distinguisher) 
                DIST="$VALUE" 
                ;;
            --run-base-lines)
                BASE=1
                ;;
            --moderator)
                MOD="$MOD $FLAG $VALUE" && MFLAG=1 
                ;; 	
            --categorical-moderator)    
                CMOD="$VALUE"&& CFLAG=1
                ;;
            --model-offset)	
                OFFSET=1
                ;;
            --model-linear-trend)
                TREND=1
                ;;
            --allow-drift)
                DRIFT=1
                ;;
            --mean-centered)
                MEAN_CENTERED=1
                ;;
            --average-size)
                average_size="$VALUE"
                ;;
            --fit-all-data)
                ALL=1
                ;;
            --bindir)
                bin_dir="$VALUE"
                ;;
            --queue-name) 
                queue_name="$VALUE" 
                ;;
            --input-dir) 
                input_dir="$VALUE" 
                ;;
            --dry-run)
		        DRYRUN=1
        		;;
    	    --exclude-percent)
        		exclude_percent="$VALUE"
        		;;
    	    --time-interval-seconds)
        		time_interval="$VALUE"
        		;;
            --cpu-server)
                cpu_server="$VALUE"
                ;;
            *)
                die "Unrecognized flag $FLAG"
        esac
    done
fi

# validation 1: check for missing arguments
test -n "$data_file"  || die "--data-file (filename.csv) is missing"
test -n "$exp_dir"    || die "--exp-dir (Experiment directory name) is missing"
test -n "$bin_dir"    || die "--bindir (Executable directory name) is missing"

# validation 2: paths and filenames
test -r "$data_file"  || die "data file $data_file not readable"
}

###########################################################################
# * Parse a comma-separated list of nonempty substrings (all packed into $2),
#   and print them, interleaving a flag string (in arg $1) before each.
#   Although the list in $2 may contain whitespace or extraneous commas,
#   those characters will be discarded.  Also most punctuation is discarded.
# * This parses commas using a robust technique described in item 47 of the
#   following page (as of May 2015):
#   http://mywiki.wooledge.org/BashPitfalls
# * Usage:  FLAG_STRING COMMA_SEP_LIST
# * Example:  parse_list "--pet" "cat,dog,fish"
#             Sent to standard output:  --pet cat --pet dog --pet fish
#             parse_list "--pet" ",cat ,do g,,fish, ," yields the same thing.
# * We have to use Bash 4.2; the "declare -n" nameref feature is unavailable.
# * If $1 does not need quoting then output is suitable for word-splitting.
#   This is NOT a robust way to generate an output array, but it is adequate.
parse_list() {
    # 1) Remove all whitespace and most punctuation, and add a trailing comma.
    #    The final comma ensures POSIX compliance, because IFS is supposed
    #    to be a field terminator.
    local datum inlist="$(sanit <<<"$2"),"
    # 2) Collapse chains of commas, if any.
    while [[ "$inlist" =~ ,, ]] ; do inlist="${inlist//,,/,}" ; done
    [[ "$inlist" = , ]] && inlist=""
    # 3) Separate list items using comma as field-termination, robustly.
    local -a iarray oarray=()
    IFS=, read -ra iarray <<< "$inlist"
    # 4) Interleave the flag.
    for datum in "${iarray[@]}"
    do [[ -n "$datum" ]] && oarray+=("$1=$datum")
    done
    # 5) Write to stdout with space separation -- not very robust.
    printf "%s " "${oarray[@]}"
}

# * Run an executable synchronously.  Redirect output streams to log directory.
runlog() {
    printf >>"$log_dir/runlog.log" "%s\n" "$*"
    local exen="$1"
    shift
    "$bin_dir/$exen" >>"$log_dir/$exen.stdout" 2>>"$log_dir/$exen.stderr" \
        "$@" || die "$exen failed to run"
}

# * find the location of a specific string 
strindex() { 
    x="${1%%$2*}"
    [[ $x = $1 ]] && echo -1 || echo ${#x}
}

# * Generate auxiliary string of moderating factors for part 1 of $prfn.
use_mgv_str() {
    [[ $1 != "" ]] \
        && printf "    using these moderator(s): %s\n" "${moderator}"
    [[ -n "${categorical_mod}" ]] \
        && printf "    using this categorical moderator: %s\n" \
                  "${categorical_mod}"
}

# * get the observable string 
get_observables() {
   obs_str=`grep "observable=" $1`
   indx=$(strindex $obs_str "=")
   let indx=$indx+1
   echo ${obs_str:$indx}
}

generate_params_list() {
    local fold_log_dir=$1
    local fold_exec_file=$2
    local num_cores=$3
    local name=$4
    local out_dir=$5
    hostname=`hostname`
    if [[ "$hostname" == *elgato* ]]
    then
        echo "bsub -R span[ptile=$num_cores] -q medium -n $num_cores \
         -o $fold_log_dir/out.txt -e $fold_log_dir/err.txt \
         -J $name $fold_exec_file >> $out_dir/../job_ids" >> $PARAM_LIST

    elif [[ "$hostname" == *vision* || "$hostname" == *v11* || "$hostname" == *bayes* ]]
    then 
        echo "$fold_exec_file >> $out_dir/job_ids" >> $PARAM_LIST

    elif [[ "$hostname" == *tacc* ]]	#Stampede
    then
        #declare -ir max_infer_mins=$(( SLURM_TACC_RUNLIMIT_MINS - 5 ))
        declare -ir max_infer_mins=$(( 1200 - 5 ))
        echo "$fold_exec_file --maximum-running-minutes=$max_infer_mins \
            1>$fold_log_dir/out.txt 2>$fold_log_dir/err.txt"  >> $PARAM_LIST
        #echo "$fold_exec_file 1>$fold_log_dir/out.txt 2>$fold_log_dir/err.txt"  >> $PARAM_LIST

    elif [[ "$hostname" == *ocelote* ]]
    then
        mkdir -p $PARAM_LIST
        echo "#!/bin/bash" >> $PARAM_LIST/$name.sh
        echo "#PBS -W group_list=kobus" >> $PARAM_LIST/$name.sh
        #echo "#PBS -l jobtype=smp_small" >> $PARAM_LIST/$name.sh
        echo "#PBS -N $name" >> $PARAM_LIST/$name.sh
        echo "#PBS -q $queue_name" >> $PARAM_LIST/$name.sh
        echo "#PBS -l select=$num_cores:ncpus=8:mem=15gb" >> $PARAM_LIST/$name.sh
        echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >> $PARAM_LIST/$name.sh
        echo "$fold_exec_file 1>$fold_log_dir/out.txt  2>$fold_log_dir/err.txt" \
            >> $PARAM_LIST/$name.sh
    else
        echo "Unknown Host"
    fi
}

run_all_folds() {

more_options=$1		

# Debug! For real runs do not override the number of iterations!! Options
# over-ride config files!! Use them sparingly!! The num-iteration options might
# be useful for debugging, but better to go through config files. 
#
# basic_options="-C $config_fn -D $data_dir --fold-info-dp=$fold_dir $more_options --train-num-iterations=1 --test-num-iterations=1 --turn-off-sampling-logging --turn-off-trace-logging"
#
# We can only access turning off loging on the command line. We are assuming
# that anyone who wants loging will be running the code more directly, and so we
# disable logging here. 
#
basic_options="-C $config_fn -D $data_dir --fold-info-dp=$fold_dir $more_options --turn-off-sampling-logging --turn-off-trace-logging"


model_name=`$bin_dir/compute_model_name -C $config_fn $OBS $more_options`
if [[ $model_name == "" ]]
then
    DIE "error in computing model name"
fi

num_fold=`ls $fold_dir/fold* -d | wc -l`

#TODO determine the cores based on the number of dyads

out_dir=$model_dir/$model_name
mkdir -p $out_dir || DIE "cannot create model directory $model_name"
local exec_file="$bin_dir/compTIES $basic_options -O $out_dir"

if [[ $input_dir != "" ]]
then
    exec_file=$exec_file" --input-dir=$input_dir/models/$model_name"
fi
echo "exec_file: "$exec_file

for fold in `seq 1 $num_fold`
do
    fold_list_dir=`printf $fold_dir/"fold-%02d" $fold`
    num_cores=`cat $fold_list_dir/testing-list.txt | wc -l`
    if [[ "$num_cores" == "" ]]
    then
        num_cores=6 
    fi
    fold_exec_file="$exec_file --fold=$fold"
    name=$model_name"_"$fold
    fold_log_dir=`printf $log_dir/$model_name/"fold-%02d" $fold`
    mkdir -p $fold_log_dir || DIE "cannnot create log directory $fold_log_dir"
    #bsub -R span[ptile=$num_cores] -q $queue_name -n $num_cores \
         #-o $fold_log_dir/out.txt -e $fold_log_dir/err.txt \
         #-J $name $fold_exec_file >> $out_dir/job_ids
    generate_params_list "$fold_log_dir" "$fold_exec_file" \
                         "$num_cores" "$name" "$out_dir"
done
}

load_modules() {

hostname=`hostname`

# set up modules 
# Load module intel and gsl in all cases.

#module load perl   || die "cannot load perl"
#module load intel  || die "cannot load intel"
#module load gsl    || die "cannot load gsl"
perl -v >/dev/null || die "System lacks perl interpreter."

# If the host machine is TACC> we have to load modules
# boost, fftw3 and setup Launcher 

}

scratch_verify(){
#------------------Scratch Space ---------------------------------
[[ -d "$SCRATCH" ]] || DIE "lacking a SCRATCH directory"
q="$SCRATCH/.test"
touch "$q" || DIE "cannot touch file $q on SCRATCH dir"
[[ -f "$q" ]] || DIE "cannot make file $q on SCRATCH dir"
rm "$q" || DIE "cannot remove file $q on SCRATCH dir"

#------------------------------------------------------------------

}

calc_launcher_ppn(){
# This function calculates the TACC_LAUNCHER_PPN based on no. of folds
# Possibly enable automatic offloading of MKL tasks to MIC (a/k/a Xeon Phi).
# * Based on https://portal.tacc.utexas.edu/tutorials/automatic-offload
# * Empirically this does not seem to help (though only lightly tested),
#   but it doesn't seem to hurt either; maybe it will help someday.
export MKL_MIC_ENABLE=1
export OMP_NUM_THREADS=16
export MIC_OMP_NUM_THREADS=240

declare -ra foldlist=("$fold_dir"/fold-[0-9][0-9])

# Determine number of processes per node that we will run.
# * If we don't match #-processes to the resources, launcher 2.0 acts weird.

(( njobs =0 )) # number of folds:  each is a regular TIES job

if [[ "${BASE}" -ne 0 ]]
then
    # average and line we are calling in the command line
    # No. of folds for MLE-CLO and MAP-CLO
    (( njobs +=  2*${#foldlist[*]} ))
fi
if [ $MFLAG -eq 1 ];            # Moderatoes are present
then
    (( njobs += ${#foldlist[*]} ))
    if [ $ALL -eq 1 ]           # Fit all Data set
    then
        (( njobs += 1 ))
    fi
fi

# The number 5 used below, is the no. of nodes available.
# The variable SLURM_NNODES should be used instead.
(( TACC_LAUNCHER_PPN = njobs / 5 )) # integer division, no remainder
readonly TACC_LAUNCHER_PPN
export TACC_LAUNCHER_PPN # This will tell the Launcher how many pids per node.

echo 'njobs=' $njobs
echo 'TACC_LAUNCHER_PPN=' $TACC_LAUNCHER_PPN

# Build paramlist file, which will be used by the Launcher module.
#: >"$CONTROL_FILE" || DIE "Cannot creat/trunc $CONTROL_FILE"

}

archive_workdir() {
    
    [[ "$attempt_to_archive" -eq 0 ]] && echo "Double-tar foiled" && return 1
#   We will never attempt this again:
    readonly attempt_to_archive=0
    
    DATE=`date +%Y-%m-%d`
    
    local r="results-$DATE.tar"
#   If user provides a config file,
    user_cf=`ls *_user`
    
    [[ -d "data" ]]        &&  tar_include="data" 
    [[ -d "folds" ]]       &&  tar_include+=" folds" 
    [[ -d "log" ]]         &&  tar_include+=" log" 
    [[ -d "models" ]]      &&  tar_include+=" models"
    [[ -d "plots" ]]       &&  tar_include+=" plots"
    [[ -d "results" ]]     &&  tar_include+=" results" 
#    [[ -e "paramlist" ]]   &&  tar_include+=" paramlist"
    [[ -e "$user_cf" ]]    &&  tar_include+=" config_user"
    [[ -e "config" ]]      &&  tar_include+=" config" 

    tar -cf $r $tar_include && bzip2 $r
    local -a d=("data" "folds" "log" "models" "plots" "results" "paramlist" "$user_cf")
    for j in ${!d[*]}
      do [[ -d ${d[j]} || -f ${d[j]} ]] || unset d[j]
    done
    [[ ${#d[@]} -gt 0 ]] && chmod -R u+wX "${d[@]}" && rm -rf "${d[@]}"
    # Add to delete bin-dir.
    #rm -rf $bin_dir
}


get_val_from_config() {
#Takes 3 arguments:
#   - config_filename - Name of configuration file
#   - cy_value - Value specified in the cyverse interface
#   - option - String specifying which option it's reading

local op=""
local val=`grep -e "^$2" $1`
if [ -z "$val" ];               # No value found in config file
then
    if [ -n "$3" ];
    then
        if [[ "$2" == "observable" ]]; 
	then
	    local optobs=("$(parse_list "--$2" "$3")" )
            op="$op $optobs"
        elif [[ "$2" == "moderator" ]] ; # If observable
	then
	    local optmod=("$(parse_list "--$2" "$3")" )
            op="$op $optmod --fit-all-data"

	elif [[ "$2" == "run-base-lines" ]] || [[ "$2" == "allow-drift" ]];
	then
	    if [[ "$3" == "1" ]];
            then
                op="$op --$2"
            fi
	
	elif [[ "$2" == "polynomial-degree" ]];
	then
	    if [[ "$3" == *"offset"* ]];
	    then 
		op="$op --model-offset"
	    elif [[ "$3" == *"trend"* ]];
	    then 
		op="$op --model-linear-trend"
	    fi
	else
            op="$op --$2=$3"
        fi
    else
        local nothing=1    
    fi
else				# Value found in config file
    if [[ "$2" == "observable" ]]; # If observable
    then
        IFS=$'= \t\n';arrIN=($val);unset IFS;
        i=0
        for x in "${arrIN[@]}"
        do
            if [ $((i%2)) -eq 1 ];
            then
            op="$op --$2=$x "
            fi
            i=$((i+1))
        done
    elif [[ "$2" == "moderator" ]];
    then
        IFS=$'=';arrIN=($val);unset IFS;
        local optmod=("$(parse_list "--moderator" "${arrIN[1]}")" )
        op="$op $optmod --fit-all-data"
    
    elif [[ "$2" == "allow-drift" ]];
    then
        op="$op --$2"

    elif [[ "$2" == "run-base-lines" ]];
    then 
        IFS=$'=';arrIN=($val);unset IFS;
  	if [[ "${arrIN[1]}" == "1" ]];
	then
	    op="$op --$2"
        fi
    elif [[ "$2" == "polynomial-degree" ]];
    then
	IFS=$'=';arrIN=($val);unset IFS;
	if [[ "${arrIN[1]}" == "1" ]];
	then
	    op="$op --model-linear-trend"
	elif [[ "${arrIN[1]}" == "0" ]];
	then
	    op="$op --model-offset"
	fi
    else	
        IFS=$'=';arrIN=($val);unset IFS;
        op="$op --$2=${arrIN[1]}"
    fi
    
fi
echo $op
}
