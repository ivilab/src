#!/bin/bash

if [[ $# -lt 4 ]]
then
    echo "Usuage $0 exp-dir list-fp data-dir max-num-jobs [READ]"
    exit 0
fi

exp_dir=$1
list_fp=$2
data_dir=$3
max_num_jobs=$4
input_dir=$5

#data_dir="/net/v04/data/jguan1/ties/physio_individual_ibi_hr_standardized/"
#data_dir="/net/v04/data/jguan1/ties/individuals/"
#data_dir="/net/v07/space/jguan1/exp/ties/synthetic/01/data/"
#data_dir="/net/v07/space/jguan1/exp/ties/synthetic/times/01/data/"
#data_dir="/net/v04/data/jguan1/ties/sythetic_new/data/"
out_dir_base="$exp_dir/$run_name/"
bindir=`../../Make/scripts/echo_machine`
cur_dir=`pwd -P`
#rand_seed=$RANDOM
#echo "random-seed=$rand_seed" > temp
#cat temp >> $out_dir_base/config
#rm temp

#for ((couple_id=2; couple_id<=46; couple_id++))
#for data_file in `ls $data_dir`
while read line 
do
    #echo $data_file
    #filename="${data_file%.*}"
    filename=`printf "%04d" $line`
    echo $filename
    out_dir="$out_dir_base/$filename"
    if [[ ! -d $out_dir ]]
    then
        echo $out_dir
        mkdir -p $out_dir
    fi
    echo $out_dir
    num_jobs=`ps wax | grep "gcc/lss_mh" | wc -l `
    echo $num_jobs
    while [[ $num_jobs -gt $max_num_jobs ]]
    do
        sleep 10s
        echo "checking now"
        num_jobs=`ps waux | grep "lss_mh" | wc -l `
        echo $num_jobs
    done
    if [[ $# -eq 5 ]];
    then
        time $cur_dir/../$bindir/prod/g++_gcc/lss_mh -F "$data_dir/$filename.txt" -C $out_dir_base/config --input-dir=$input_dir/$filename $out_dir_base > $out_dir/out.txt 2>&1 &
    else
        time $cur_dir/../$bindir/prod/g++_gcc/lss_mh -F "$data_dir/$filename.txt" -C $out_dir_base/config $out_dir_base > $out_dir/out.txt 2>&1 &
    fi
    sleep 5s
done < $list_fp

wait;

