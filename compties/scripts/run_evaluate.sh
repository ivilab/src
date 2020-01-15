#!/bin/bash

if [[ $# -lt 3 || $# -gt 4 ]]
then
    echo "Usuage $0 exp-dir list-fp data-dir [visualize]"
    exit 0
fi

exp_dir=$1
list_fp=$2
data_dir=$3

error_mode="rms"
out_dir="$exp_dir/errors"
rm -rf $out_dir
mkdir $out_dir

bindir=`../../Make/scripts/echo_machine`
cur_dir=`pwd -P`
host=`hostname | grep "elgato" | wc -l`
if [[ $host -eq 1 ]]
then
    exec_dir="$cur_dir/../$bindir/prod/icpc_icc/"
else
    exec_dir="$cur_dir/../$bindir/prod/g++_gcc/"
fi

if [ $# -eq 4 ]
then
    $exec_dir/evaluate -E $exp_dir --training-percent=0.8 -L $list_fp $data_dir --visualize
else
    $exec_dir/evaluate -E $exp_dir --training-percent=0.8 -L $list_fp $data_dir 
fi

