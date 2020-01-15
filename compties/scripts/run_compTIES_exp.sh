#!/bin/bash 

# run_compTIES_exp.sh 2016-10-13 05:20:20Z chinmaib $
# Derived from ties/iplant_middleware/tacc/parallel_complete_ties.sh by predoehl.
# Adapted to run on UA HPC and other servers to perform a basic complete run, i.e., 
# TIES model, plus easy  baselines. 
# It requires a number of arguments in a paired form like "--flag=VALUE"
# please see the output of --help.

# NOTE: If running through command line, please run using 

#################################################################################

# I do not know what directory I am in, or where I really want this output.
exec 21> /tmp/run_compTIES.log.dry_run 
export BASH_XTRACEFD="21"
set -x

hostname=`hostname`

# Utils.sh & default template are located in the same DIR as run_compTIES_exp.sh
utils_path=$(dirname "$0")

# If $COMPTIES_HOME is not set, then search for utils.sh and template in same DIR 
if [[ -z $COMPTIES_HOME ]]
then
    readonly def_conf_template="$utils_path/compTIES.conf.template"
    source $utils_path/utils.sh
else
    readonly def_conf_template="$COMPTIES_HOME/scripts/compTIES.conf.template"
    source $COMPTIES_HOME/scripts/utils.sh
fi
# Script requires arguments.
test $# -eq 0 && die "Invocation lacks required arguments"

BASE=0
OFFSET=0
TREND=0
DRIFT=0
CFLAG=0
MFLAG=0
ALL=0
MEAN_CENTERED=0
MOD_DIFF=0
MOD_AVE=0
average_size=1
exclude_percent=0
time_interval=0
DRYRUN=0

parse_options $*

BNO="basename $0"
PARAM_LIST=$exp_dir/paramlist
touch $PARAM_LIST

readonly BASE OFFSET TREND DRIFT CFLAG MFLAG ALL PARAM_LIST MEAN_CENTERED 
readonly data_file config_file bin_dir OBS DIST MOD CMOD DRYRUN
readonly exclude_percent time_interval cpu_server

# Set up the experiment dir.
# NOTE: Experiment directory is being created by the PHP script
# Write an if condition to check if the input CSV file & exp_dir exist.
# If not create exp_dir

if [[ ! -d $exp_dir ]]
then
    mkdir -p $exp_dir || die "cannot create experiment directory"
    chown -R :vision $exp_dir
    chmod -R 775 $exp_dir
fi


# set up the log directory
log_dir=$exp_dir/log
mkdir -p $log_dir || die "cannot create log directory $log_dir"

load_modules

############################## CSV RECTIFY ####################################

# rectify any newline weirdness arising from MS-Windows
# creates a folder called rectified_input & places the rectified file there.

if [[ "${data_file}" != "" ]]
then
    RIDIR=$exp_dir/rectified_input
    mkdir $RIDIR || die "cannot create directory for rectified input file"
    good_csv=$RIDIR/`basename "$data_file"`
    perl -pe "s/\r\n/\n/sg; s/\r/\n/sg" "$data_file" >$good_csv || die "Cannot rectify newlines"
    if cmp "$data_file" "$good_csv"
    then rm "$good_csv"; rmdir $RIDIR; good_csv="$data_file"
    fi
fi

############################# CONFIG FILE ######################################

config_fn=$exp_dir/config     # Name of the config file

# If a config file already exists in the exp_dir, which will be most 
# likely be the case in real run, ignore constructing config file aagain

if [[ -f $config_fn ]];
then
    echo "Config file already exists. Skipping build process"
else
    # 1st we copy the default template options to the config file.
    cat $def_conf_template > $config_fn

    # Check to see if user_config is specified. 
    # If yes, then remove all the options specified from the default template
    # Append user_config to edited config file. (Duplicates removed)
    if [[ -n $config_file ]];
    then
        echo $config_file
        while read -r line; do
            IFS=’=’ read -ra CATEGORY <<< "$line"
            # Ignoring blank lines.
            if [[  ${#CATEGORY[0]} -gt 2 ]];then
                cmd="/${CATEGORY[0]}/ d "
                sed -i "$cmd" $config_fn
            fi
        done < $config_file
        cat $config_file >> $config_fn
    fi

    # Adding observable info into config file
    # Observable can  have multiple values in the form '--observable dial'
    # Second word always contains the value. Ignore the first word.
    i=0
    if [[ -n $OBS ]];
    then
        # Remove previous observable entry  
        sed -i "/observable/ d" $config_fn
        for word in $OBS; do
            if [ $((i%2)) -eq 1 ];
            then
                printf >>$config_fn "observable=$word\n"
            fi
        i=$((i+1))
       done
    fi
    if [[ -n $DIST ]];
    then
        # Remove distinguisher
        sed -i "/distinguisher/ d" $config_fn
        printf >>$config_fn "distinguisher=$DIST\n"
    fi
    i=0
    if [[ -n $MOD ]];
    then
        # Remove previous moderator entry  
        sed -i "/moderator/ d" $config_fn
        for word in $MOD; do
            if [ $((i%2)) -eq 1 ];
            then
                printf >>$config_fn "moderator=$word\n"
            fi
        i=$((i+1))
       done
    fi
    if [ $CFLAG -eq 1 ];
    then
        # Remove categorical mod
        sed -i "/categorical-moderator/ d" $config_fn
        printf >>$config_fn "categorical-moderator=$CMOD\n"
    fi
    if [ $OFFSET -eq 1 ];
    then
        # Add Offset/Tend
        sed -i "/polynomial-degree/ d" $config_fn
        printf >>$config_fn "polynomial-degree=0\n"
    fi
    if [ $TREND -eq 1 ];
    then
        sed -i "/polynomial-degree/ d" $config_fn
        printf >>$config_fn "polynomial-degree=1\n"
    fi
    if [ $DRIFT -eq 1 ];
    then
        sed -i "/allow-drift/ d" $config_fn
        printf >>$config_fn "allow-drift=\n" 
    fi
fi
############################ VERIFY OBS AND DIST PRESENT ###################

if ! grep -q "observable" $config_fn;
then
    die "observable value not specified"
fi
if ! grep -q "distinguisher" $config_fn;
then
    die "distinguisher value not specified"
fi

############################ VERIFY EXECUTABLES ##############################

# Find and verify executables
for BIN in create_folds parse_real_data compTIES compute_model_name  
do test -x "$bin_dir/$BIN" || die "$BIN not executable"
   NFSO=`(ldd "$bin_dir/$BIN" | grep "not found")`
   test -n "$NFSO" && echo "$NFSO" && die "$BIN cannot be run"
done

############################## PARSE REAL DATA ################################

data_dir=$exp_dir/data

mkdir -p $data_dir || die "cannot create data directory $data_dir"
parse_data_options=" -I $good_csv $MOD $OBS --distinguisher $DIST \
		    -O $data_dir --average-size=$average_size \
		    --exclude-percent=$exclude_percent" 
if [[ $MEAN_CENTERED -eq 1 ]]
then
    parse_data_options="${parse_data_options} --mean-centered"
fi
if [[ $MOD_DIFF -eq 1 ]]
then
    parse_data_options="${parse_data_options} --moderator-diff"
fi

if [[ $MOD_AVE -eq 1 ]]
then
    parse_data_options="${parse_data_options} --moderator-ave"
fi
echo parse_data_options: $parse_data_options
runlog parse_real_data $parse_data_options

test -d $data_dir || die "failed to create data dir $data_dir"
test -f $data_dir/ids.txt || die "no dyads"

#######################################################################
#                           CREATE FOLDS
#######################################################################

# Creating a folds directory in exp_dir call is folds
fold_dir=$exp_dir/folds
mkdir -p $fold_dir || die "cannot create folds directory $fold_dir"
options="-D $data_dir -C $config_fn -O $fold_dir"
echo "Creating the folds"
runlog create_folds $options 

test -d $fold_dir || die "failed to create folds dir $fold_dir"

model_dir=$exp_dir/models
mkdir -p $model_dir || die "cannot create models directory $model_dir"


#########################################################################
#                               DRY RUN 
#########################################################################

# Run only if --dry-run is specified.
if [[ $DRYRUN -eq 1 ]]
then
    echo "Commencing Dry Run"
    dryrun_opt="--train-num-iterations=0 --test-num-iterations=0"

else
    dryrun_opt=""
fi
	
#########################################################################
#                               BASE MODELS 
#########################################################################

# Run only if --run-base-lines is specified.
if [ $BASE -eq 1 ];
then
    # Using compute_model_name to obtain the name of the model
    # Running baseline models in command promp.    

    ############################# Average model  ##############################
    model_name=`$bin_dir/compute_model_name -C $config_fn $OBS --run-average-model`
    out_dir=$model_dir/$model_name
    mkdir -p $out_dir || die "cannot create model directory $model_name"

    echo "Running average model"
    runlog compTIES --run-average-model -C $config_fn -O $out_dir -D $data_dir 

    ############################ Linear model  ################################
    model_name=`$bin_dir/compute_model_name -C $config_fn $OBS --run-line-model`
    out_dir=$model_dir/$model_name
    mkdir -p $out_dir || die "cannot create model directory $model_name"

    echo "Running linear model"
    runlog compTIES --run-line-model -C $config_fn -O $out_dir -D $data_dir 

    ############################# MLE CLO ######################################
    echo "Creating MLE-CLO jobs"
    run_all_folds "--fit-ind-clo $dryrun_opt"

    ################################ MAP CLO ####################################
    echo "Creating MAP-CLO jobs"
    model_name=`$bin_dir/compute_model_name -C $config_fn $OBS`
    run_all_folds "$dryrun_opt"
fi

#########################################################################
#                            MODERATORS 
#########################################################################

if [ $MFLAG -eq 1 ];		# Moderators are present
then
    echo "Running moderator"
    run_all_folds "$MOD $dryrun_opt"
 
     # Fit All Data
    if [ $ALL -eq 1 ]
    then
        model_name=`$bin_dir/compute_model_name \
                    -C $config_fn $OBS $MOD --fit-all-data` || \
                    die "cannot compute model name "
        out_dir=$model_dir/$model_name
        echo "model name" $model_name

        mkdir -p $out_dir || die "cannot create model directory $model_name"
        mkdir -p $out_dir/all_data || die "cannot create directory $out_dir/all_data"
        exec_file="$bin_dir/compTIES -C $config_fn -O $out_dir -D $data_dir \
                   $MOD --fit-all-data $dryrun_opt"
        name=$model_name"_all"
        echo "name " $name

        echo "Training model parameters with all of the data"
        cur_log_dir=$log_dir/$model_name/all-data
        mkdir -p $cur_log_dir || die "cannot create log directory $cur_log_dir"
        num_cores=`cat $data_dir/ids.txt | wc -l`
        if [[ "$num_cores" == ""  || "$num_cores" > 6 ]]
        then
            num_cores=6
        fi
        generate_params_list "$cur_log_dir" "$exec_file" $num_cores "$name" \
        "$out_dir/all_data"
    fi
fi

#########################################################################
#                            SUBMITTING JOBS
#########################################################################

if [[ "$hostname" == *elgato* ]]
then
    # elgato, still testing 
    bash $PARAM_LIST || die "error when submitting jobs"

elif [[ "$hostname" == *vision* ]]
then
    # Vision Mostly Dry Run
    bash $PARAM_LIST || die "error when submitting jobs on vision"
    # /usr/local/bin/parallel -P 8 -a $PARAM_LIST || die "error when submitting jobs on vision"

elif [[ "$hostname" == *v11* ]]
then
    # bash $PARAM_LIST || die "error when submitting jobs on v11"
    /usr/local/bin/parallel -P 32 -a $PARAM_LIST || die "error when submitting jobs on v11"

elif [[ "$hostname" == *bayes* ]]
then
    /usr/local/bin/parallel -P 24 -a $PARAM_LIST || die "error when submitting jobs on bayes01"

elif [[ "$hostname" == *stampede* ]]
then
    $TACC_LAUNCHER_DIR/paramrun SLURM $EXECUTABLE $WORKDIR \
    $PARAM_LIST $PHI_WORKDIR $PHI_CONTROL_FILE
    
elif [[ "$hostname" == *ocelote* ]]
then
    # ocelote, still testing
    cd $log_dir
    for file in `ls $PARAM_LIST`
    do 
        echo $file
        sleep 1s
        qsub $PARAM_LIST/$file || die "error when submitting jobs"
    done
fi
#########################################################################
#                            RESULTS & PLOTS
#########################################################################

echo "Starting to aggregate results $(date)"
$COMPTIES_HOME/scripts/generate_results.sh $exp_dir $bin_dir
echo "Generate results completed on $(date)"


##########################################################################
#                        SUCCESS FLAG
#########################################################################

if [[ $DRYRUN -eq 1 && ! -f $exp_dir/FAILURE_REPORT.txt  ]]
then
    touch $exp_dir/DRY_RUN_SUCCESS

    [[ -e $exp_dir/paramlist ]] && cp $exp_dir/paramlist $exp_dir/paramlist.dry_run
    [[ -e $exp_dir/config ]] && cp $exp_dir/config $exp_dir/config.dry_run
    [[ -e /tmp/run_compTIES.log.dry_run ]] && cp /tmp/run_compTIES.log.dry_run $exp_dir/run_compTIES.log.dry_run 

    # I think this is stale and/or wrong --- what directory are we in anyway? DELETE? 
    #
    # Clean Up Process
    [[ -d "data" ]]        &&  rm_include="data"
    [[ -d "folds" ]]       &&  rm_include+=" folds"
    [[ -d "log" ]]         &&  rm_include+=" log"
    [[ -d "models" ]]      &&  rm_include+=" models"
    [[ -d "plots" ]]       &&  rm_include+=" plots"
    [[ -d "results" ]]     &&  rm_include+=" results"
    [[ -e "paramlist" ]]   &&  rm_include+=" paramlist"
    [[ -e "primary_results.txt" ]] && rm_include+=" primary_results.txt"
fi
 
echo $cpu_server >> $exp_dir/CPU_SERVER
	
# Change directory persmissions 
chmod -R 755 $exp_dir/*
