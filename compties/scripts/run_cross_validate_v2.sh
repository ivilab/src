#!/bin/bash -l

LD_PRELOAD="/usr/lib64/libtcmalloc.so"

if [[ $# -ne 2 ]]
then
    echo "Usuage $0  out-dir-base run-name "
    exit 0
fi
run_name=$2
echo $run_name

data_dir="/net/v04/data/jguan1/ties/individuals/"
out_dir_base=$1
sub_out_dir="$out_dir_base/$run_name/"
cur_dir=`pwd -P`

exec_file="$cur_dir/../${MACHINE}/prod/g++_gcc/cross_validate"
list_fp="$out_dir_base/lists.txt"

base_config=$sub_out_dir/config 
if [ ! -f $base_config ]
then 
    mkdir -p $sub_out_dir
    cp $out_dir_base/config $base_config
    num_jobs=0
    rand_seed=$RANDOM
    echo "random-seed=$rand_seed" >> $sub_out_dir/config
fi

config_file=$sub_out_dir/config
let "i=0"

# bmi-dependent parameters 
for beta in {0..5..2}
do 
    out_dirs[$i]=`printf "$sub_out_dir/beta-%02d" $beta`
    mkdir -p ${out_dirs[i]}
    if [[ ! -f ${out_dirs[i]}/config ]]
    then
        cp $config_file ${out_dirs[i]}/config
        echo "beta-indices=$beta" >> ${out_dirs[i]}/config 
        let "beta_2=$beta+1"
        echo "beta-indices=$beta_2" >> ${out_dirs[i]}/config 
        echo "init-beta-coefs=0.00001" >> ${out_dirs[i]}/config 
        echo "init-beta-coefs=0.00001" >> ${out_dirs[i]}/config 
    fi
    let "i=$i+1"
done

for gamma in {0..5..2}
do 
    out_dirs[$i]=`printf "$sub_out_dir/gamma-%02d" $gamma`
    mkdir -p ${out_dirs[i]}
    if [[ ! -f ${out_dirs[i]}/config ]]
    then
        cp $config_file ${out_dirs[i]}/config
        echo "gamma-indices=$gamma" >> ${out_dirs[i]}/config 
        let "gamma_2=$gamma+1"
        echo "gamma-indices=$gamma_2" >> ${out_dirs[i]}/config 
        echo "init-gamma-coefs=0.00001" >> ${out_dirs[i]}/config 
        echo "init-gamma-coefs=0.00001" >> ${out_dirs[i]}/config 
    fi
    let "i=$i+1"
done

for (( i = 0; i < ${#out_dirs[@]}; i++ ))
#for (( i = 0; i < 3; i++ ))
do 
    out_dir=${out_dirs[i]}
    #echo cp $list_fp $out_dir/lists.txt 
    #cp $list_fp $out_dir/lists.txt 
    echo $out_dir
    #echo "input-dir=${out_dirs[i]}" >> ${out_dirs[i]}/config
    #num_jobs=`ps wax | grep "cross_validate" | wc -l `
    #echo $num_jobs
    #while [ $num_jobs -gt 5 ]
    #do
    #    sleep 10m
    #    echo "checking now"
    #    num_jobs=`ps wax | grep "cross_validate" | wc -l `
    #    echo $num_jobs
    #done
    date
    time $exec_file $out_dir -D $data_dir -C $out_dir/config -L $out_dir_base/lists.txt >& $out_dir/out.txt #2>&1
    date
done
