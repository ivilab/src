#!/bin/bash
# Aggregate the errors for the cross-validated results
# Chinmai: 16/12 
# Update script to look at $COMPTIES/scripts 

source ./utils.sh

if [[ $# -ne 2 ]]
then
    echo "Usage $0 exp-dir [bin_dir] "
    exit 0
fi

#module load python

exp_dir=$1
cur_dir=`pwd -P`
bin_dir=$2
log_dir=$exp_dir/log
config_fn=$exp_dir/config
fold_dir=$exp_dir/folds
model_dir=$exp_dir/models
data_dir=$exp_dir/data

# Get the observables from the config file
num_observ=`grep "observable" $config_fn | wc -l`
obs_str=`grep "observable" $config_fn | cut -d"=" -f2`
observable=($obs_str)
distinguisher=`grep "distinguisher" $config_fn | cut -d"=" -f2`

# Get the categorical moderator from the script file 
cat_mod_str=`grep "categorical-moderator" $config_fn | cut -d"=" -f2`
categorical_mod=($cat_mod_str)

# Check the validility of the script
for exen in ties_plots.py #ties_ideal_mod.py
do [[ -x "$bin_dir/$exen" ]] || DIE "$exen not runnable 2"
done


# check the plots directory.
readonly plot_dir="$exp_dir/plots"
if [[  -d $plot_dir ]]
then
    rm -rf $plot_dir
fi 
mkdir -p $plot_dir  || DIE "mkdir $plot_dir"

###########################################################################
#                       Construct the options
###########################################################################
declare -ar plot_logdir=("--log-dirname" "$log_dir")
declare -a opt_plot=("--data-dir" "$data_dir"
                    "--distinguisher" "${distinguisher}"
                    "--output-dir-plots" "$plot_dir")
if [[ "$catgorical_mod" != "" ]]
then
    opt_plot+=("--categorical-mod" "${categorical_mod}")
fi

plot_baseline_dir=$plot_dir/"baselines"

echo "Coming here. All good"
echo ${opt_plot[@]}
###########################################################################
#                   Generate plots showing fit quality
###########################################################################
for dir in `ls $model_dir`
do
    if [[ "$dir" == "average" || "$dir" == "line" ]]
    then
        # baseline directory plots  
        if [[ ! -d $plot_baseline_dir ]]
        then
            mkdir -p $plot_baseline_dir || DIE "mkdir $plot_baseline_dir"
        fi
        for obs in "${observable[@]}"
        do
            res_dir=$model_dir/$dir/$obs
            local_opt_plot=("--output-dir-plots" "$plot_baseline_dir/$dir/$obs"
                       "--single-fold="
                       "--model-name" $dir)
            ## Generate individual plots 
            runlog ties_plots.py "--output-dir-model" "$res_dir" \
                    "${opt_plot[@]}" "${local_opt_plot[@]}" "${plot_logdir[@]}"
        done
    elif [[ "$dir" == *moderator* ]]
    then
        mod_indx=$(strindex $dir "moderator-")
        moderator_str="moderator-"
        let mod_indx=$mod_indx+${#moderator_str}
        moderator=${dir:$mod_indx}
        ## Generate plots for the moderator 
        res_dir=$model_dir/$dir
        local_opt_plot=("--output-dir-plots" "$plot_dir/moderator-$moderator"
                   "--model-name" $dir)
        runlog ties_plots.py "--output-dir-model" "$res_dir" \
                    "${opt_plot[@]}" "${local_opt_plot[@]}" "${plot_logdir[@]}"

        if [[ -d $dir/all_data ]]
        then
            plot_dir="$plot_dir/moderator-$moderator/idealized_interaction"
            runlog ties_ideal_mod.py "--output-dir-plots" "$plot_dir" \
                        "--charmod-dirname" "$dir/all_data" "${plot_logdir[@]}"
        fi
    else
        ## Generate plots for the other baselines 
        res_dir=$model_dir/$dir
        if [[ ! -d $plot_baseline_dir ]]
        then
            mkdir -p $plot_baseline_dir || DIE "mkdir $plot_baseline_dir"
        fi
        local_opt_plot=("--output-dir-plots" $plot_baseline_dir/$dir
                   "--model-name" $dir)
        runlog ties_plots.py "--output-dir-model" "$res_dir" \
                    "${opt_plot[@]}" "${local_opt_plot[@]}" "${plot_logdir[@]}"
    fi
done

