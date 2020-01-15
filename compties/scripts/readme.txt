/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author).
|
|  Personal and educational use of this code is granted, provided that this
|  header is kept intact, and that the authorship is not misrepresented, that
|  its use is acknowledged in publications, and relevant papers are cited.
|
|  For other use contact the author (kobus AT cs DOT arizona DOT edu).
|
|  Please note that the code in this file has not necessarily been adequately
|  tested. Naturally, there is no guarantee of performance, support, or fitness
|  for any particular task. Nonetheless, I am interested in hearing about
|  problems that you encounter.
|
| Authors:
|     Jinyan Guan
|
* =========================================================================== */
$Id: readme.txt 21009 2017-01-01 05:13:14Z jguan1 $

This directory contains the scripts for running experiments on the vision lab
machines, the UA HPC machines, and UA Elgato. 


1) ./generate_results.sh: Compute the errors for all the models inside the
exp-dir directory. It will generate a primary_results.txt file that contains
the summary of all the models.  

Usuage ./run_cv_mean.sh exp-dir 

where exp-dir is the directory contains the following subfolders:
/data
/folds
/models
/data

2) ./run_fold_single_elgato.sh: Launch cross-validation for an experiment

Usuage ./run_fold_single_elgato.sh out-dir [data]

where out-dir is the experiment directory, which must contains a config file
and a lists.txt file, and data is the parsed data directory from the .csv file 

3) ...
