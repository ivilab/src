#!/bin/csh
### script to run 4 serial jobs using  4 cores on htc using queue windfall or standard

### this script can be modified to use one complete, 12-core node on the htc or cluster compute systems
### this script will not work to run more than 12 serial jobs across multiple nodes

### beginning of line, three pound/cross-hatch characters indicate comment
### beginning of line #PBS indicates an active PBS command/directive
### use ###PBS and #PBS to deactivate and lines without removing them from script

### Refer to UA Batch System website for system and queue specific limits (max values)
### Minimize resource requests (ncpus, mem, walltime, cputime, etc) to minimize queue wait delays

### Set the job name
#PBS -N cross

### Request email when job begins and ends
###PBS -m bea

### Specify email address to use for notification.
#PBS -M jguan1@email.arizona.edu

### Specify the PI group for this job
### List of PI groups available to each user can be found with "va" command
#PBS -W group_list=kobus

### Set the queue for this job as windfall or standard (adjust ### and #)
#PBS -q standard
###PBS -q windfall

### Set the jobtype for this job (serial, small_mpi, small_smp, large_mpi, large_smp)
### jobtype=serial submits to htc and can be automatically moved to cluster and smp
### Type parameter determines initial queue placement and possible automatic queue moves
#PBS -l jobtype=smp_only

### Set the number of cores (cpus) and memory that will be used for this job
### When specifying memory request slightly less than 2GB memory per ncpus for standard node
### Some memory needs to be reserved for the Linux system processes
#PBS -l select=5:ncpus=8:mem=15gb

### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=30:00:00

### Specify total cpu time required for this job, hhh:mm:ss
### total cputime = walltime * ncpus
#PBS -l cput=1200:00:10

### Load required modules/libraries if needed (blas example)
### Use "module avail" command to list all available modules
### NOTE: /usr/share/Modules/init/csh -CAPITAL M in Modules
source /usr/share/Modules/init/csh

### cd: set directory for job execution, ~netid = home directory path
### executable command with trailing & - "

#cd  ~jguan1/src/ties/linux_x86_64_xeon/prod/g++_g++/

setenv LD_LIBRARY_PATH "/home/u2/kobus/misc/load/linux_x86_64_xeon/g++:/home/u2/kobus/misc/load/linux_x86_64_xeon/g++:/home/u2/kobus/misc/load/linux_x86_64_xeon/:/uaopt/boost/1.48.0/lib:/uaopt/fftw/3.3/lib:/uaopt/gsl/1.15/lib:/uaopt/intel/2012.0.032/mkl/lib/intel64:/uaopt/intel/2012.0.032/lib/intel64:/uaopt/intel/2012.0.032/lib/ia32:/usr/local/lib64:/usr/local/lib:/usr/lib64:/usr/lib:/lib64:/lib"

###setenv exp_dir $1 
###setenv run_name $2
###setenv out_dir "/xdisk/jguan1/exp/ties/bmi_dependent_1.0/11.04/$indir/" 
setenv out_dir "$outdir" 
setenv config_fp $out_dir/config
setenv data_dir "/xdisk/jguan1//data/ties/individuals/" 
setenv bindir "/home/u20/jguan1/src/ties/linux_x86_64_xeon/prod/g++_g++/"

date
/usr/bin/time $bindir/cross_validate -D $data_dir -C $config_fp -L $out_dir/lists.txt $out_dir >& $out_dir/out.txt & 
date

wait

