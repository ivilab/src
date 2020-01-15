#!/bin/bash
module load gsl
module load intel
#source ./set_env.sh

#if [[ $# -lt 3 ]]
if [[ $# -lt 3 || $# -gt 4 ]]
then
    echo "Usuage $0 exp-dir list-fp data-dir input-dir"
    exit 0
fi
exp_dir=$1
list_fp=$2
data_dir=$3
input_dir_base=$4

#data_dir="/net/v04/data/jguan1/ties/physio_individual_ibi_hr_standardized/"
#data_dir="/net/v04/data/jguan1/ties/individuals/"
#data_dir="/space/jguan1/exp/ties/synthetic/drift_07/data"
#data_dir="/net/v04/data/jguan1/ties/sythetic_new/data/"
#out_dir_base="$exp_dir/$run_name/"
out_dir_base=$exp_dir
bindir=`../../Make/scripts/echo_machine`
cur_dir=`pwd -P`
#rand_seed=$RANDOM
#echo "random-seed=$rand_seed" > temp
#cat temp >> $out_dir_base/config
#rm temp

#for data_file in `ls $data_dir`
while read line 
do
    #echo $data_file
    #filename="${data_file%.*}"
    filename=`printf "%04d" $line`
    #echo $filename
    out_dir="$out_dir_base/$filename"
    if [[ ! -d $out_dir ]]
    then
        mkdir -p $out_dir
    fi
    echo $out_dir
    exec_file="$cur_dir/../$bindir/prod/icpc_icc/lss_mh -F "$data_dir/$filename.txt" -C $out_dir_base/config $out_dir_base"
    if [[ $# -eq 4 ]]
    then
        input_dp=$input_dir_base/$filename
        exec_file="$exec_file --input-dir=$input_dp"
    fi
    bsub -q "medium" -R gpu -n 1 -o $out_dir/out.txt -e $out_dir/err.txt -J "lss-mh" $exec_file  >> $exp_dir/job_ids
    #bsub -q "windfall" -n 1 -o $out_dir/out.txt -e $out_dir/err.txt -J "lss-mh" $exec_file  >> $exp_dir/job_ids
    sleep 1s
done < $list_fp
