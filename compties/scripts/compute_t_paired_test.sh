#!/bin/bash
# Aggregate the errors for the cross-validated results
if [[ $# -ne 2 ]]
then 
    echo "Usuage $0 exp-1 (baselines) exp-2"
fi

module load matlab
matlab -nosplash -nodisplay -nojvm -r "compute_t_paired_test('$1', '$2'), quit()" 
