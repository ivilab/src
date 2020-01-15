#!/bin/bash
if [[ $hostname == elgato* ]]
then 
    module load intel
    module load gsl
    module load boost
    module load fftw3
fi

if [[ $# -ne 3 ]]
then
    echo "Usuage $0  fold-dir exp-dir config-file"
    exit 0
fi


fold_dir=$1
exp_dir=$2
config_fp=$3

cur_dir=`pwd -P`
bindir=`../../Make/scripts/echo_machine`
hostname=`hostname`
if [[ $hostname == elgato* ]]
then 
    module load intel
    module load gsl
    exec_file="$cur_dir/../$bindir/prod/icpc_icc/aggregate_fold_errors"
else
    exec_file="$cur_dir/../$bindir/prod/g++_gcc/aggregate_fold_errors"
fi

if [ ! -d $exp_dir/errors ]
then
    $exec_file -C $config_fp --fold-info-dp $fold_dir --write-latex-error --distinguisher=# --output-dir=$exp_dir  
fi
