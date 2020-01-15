#!/bin/bash
module load intel
module load gsl
#source ./set_env.sh


if [[ $# -lt 3 || $# -gt 4 ]]
then
    echo "Usuage $0 out-dir data fold-dir"
    exit 0
fi
out_dir=$1
data_dir=$2
fold_dir=$3
more_options=$4
bindir=`../../Make/scripts/echo_machine`
cur_dir=`pwd -P`

exec_dir="$cur_dir/../$bindir/prod/icpc_icc/"
config_fp=$out_dir/"config"
options="-D $data_dir -C $config_fp $more_options"

if [ ! -d $fold_dir ]
then
    echo "Creating the fold dir " $fold_dir
    echo $exec_dir/create_folds $options -O $fold_dir
    $exec_dir/create_folds $options -O $fold_dir
fi

model_name=`$exec_dir/compute_model_name $options`
out_dir=$out_dir/"models"/${model_name}
echo $out_dir
mkdir -p $out_dir
    
# submit jobs for each fold 
echo $num_fold
sleep 2s
num_fold=`ls $fold_dir/fold* -d | wc -l`
#for fold in `seq 1 $num_fold`
for fold in `seq 2 2`
do
    exec_file="$exec_dir/compTIES --fold=$fold $options -O $out_dir --fold-info-dp=$fold_dir"
    name=$model_name"_"$fold
    log_dir=`printf $out_dir/"fold-%02d" $fold`
    mkdir -p $log_dir
    num_cores=5
    fixed_prior=`grep "fixed_mean" $config_fp | wc -l`
    if [[ $fixed_prior -eq 1 ]]
    then
        num_cores=5
    fi
    echo $fold
    echo $exec_file
    #exec $exec_file >& /dev/null &
    bsub -R span[ptile=$num_cores] -q "medium" -n $num_cores -o $log_dir/out.txt -e $log_dir/err.txt -J $name $exec_file  >> $out_dir/job_ids
    #bsub -R span[ptile=$num_cores] -q "windfall" -n $num_cores -o $log_dir/out.txt -e $log_dir/err.txt -J $name $exec_file  >> $out_dir/job_ids
    sleep 1s
done
