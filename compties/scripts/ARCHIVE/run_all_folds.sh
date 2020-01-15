#!/bin/bash


if [[ $# -lt 1 || $# -gt 4 ]]
then
    echo "Usuage $0 out-dir bin-dir queue-name options"
    exit 0
fi
out_dir=$1
bin_dir=$2
queue_name=$3
more_options=$4
model_dir=$out_dir/models
data_dir=$out_dir/data
fold_dir=$out_dir/folds
config_fn=$out_dir/config
log_dir=$out_dir/log

source utils.sh

if [[ "$bin_dir" == "" ]]
then
    MACHINE=`../../Make/scripts/echo_machine`
    cur_dir=`pwd -P`
    bin_dir="$cur_dir/../$MACHINE/prod/icpc_icc/"
fi
if [[ "$queue_name" == "" ]]
then
    queue_name="medium"
fi


if [ ! -d $fold_dir ]
then
    echo "Creating new fold dir $fold_dir"
    runlog create_folds $basic_options -O $fold_dir
fi

run_all_folds "$more_options"

