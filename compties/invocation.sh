# Trying to figure out invocation
# Author: Adarsh Pyarelal

time make parse_real_data
time make create_folds
time make compTIES

experiment_dir=experiment
data_dir=experiment/co18small_processed
config_file=input/config_adarsh_testing
fold_dir=$experiment_dir/folds
input_csv=input/co18small.csv
observable=dial
distinguisher=sexm
moderator=bmiave
output_dir=experiment/output
mkdir -p $output_dir $fold_dir $data_dir
train_num_iterations=10
test_num_iterations=10

echo "Parsing CSV"
./parse_real_data \
  -I $input_csv\
   -O $data_dir\
  --observable $observable\
  --distinguisher $distinguisher\
  --moderator $moderator

echo "Creating folds"
./create_folds -D $data_dir -C $config_file -O $fold_dir 

echo "Running compTIES"
./compTIES \
  -D $data_dir\
  -C $config_file\
  -O $output_dir\
  --fold-info-dp=$fold_dir\
  --moderator $moderator\
  --turn-off-sampling-logging\
  --turn-off-trace-logging\
  --train-num-iterations=$train_num_iterations\
  --test-num-iterations=$test_num_iterations
