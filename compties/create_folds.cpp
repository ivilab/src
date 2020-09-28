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

/* $Id: create_folds.cpp 25834 2020-09-28 17:23:27Z kobus $ */

#include "dbn_cpp/data.h"
#include "dbn_cpp/likelihood.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/lss_set_sampler.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#include "dbn_cpp/cross_validate_util.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif
    Ties_experiment exp = experiment_from_cl_options(argc, argv);

    // Set random seed 
    ivi_c::ivi_seed_rand(exp.rand_seed, exp.rand_seed);
    ivi_c::ivi_seed_rand_2(exp.rand_seed);
    ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);
    srand(exp.rand_seed);

    std::string out_dp = exp.out_dp;
    std::cout << out_dp << std::endl;
    IVI(ivi_mkdir(out_dp.c_str()));
    size_t K;
    // no groups 
    if(exp.data.grouping_var == "")
    {
        exp.data.id_list_fp.clear();
        exp.data.id_list_fp.push_back(
                    std::string(exp.data.data_dp + "/ids.txt"));
        K = create_fold(out_dp, exp.data.id_list_fp[0], exp.run.num_folds);
    }
    else
    {
        // If the grouping_var is set, then the following directory structure is
        // created 
        // fold[N] - [CATEGARY-1-training-list.txt]
        //         - [CATEGARY-2-training-list.txt]
        //         - [CATEGARY-3-training-list.txt]
        //         ....
        //         - [CATEGARY-1-testing-list.txt]
        //         - [CATEGARY-2-testing-list.txt]
        //         - [CATEGARY-3-testing-list.txt]
        //         ....
        //
        std::vector<std::string> data_groups = get_data_group_dirs(exp.data.data_dp);
        size_t num_groups = data_groups.size();
        std::vector<std::string> list_groups(num_groups);
        for(size_t g = 0; g < num_groups; g++)
        {
            std::string list_fp = data_groups[g] + "/ids.txt";
            list_groups[g] = list_fp;
            std::cout << " list: " << list_fp << std::endl;
        }
        size_t cur_k = 0;
        for(size_t g = 0; g < num_groups; g++)
        {
            size_t found = data_groups[g].find_last_of("/\\");
            std::string group_name = data_groups[g].substr(found+1);
            cur_k = create_fold(out_dp, list_groups[g], 
                                exp.run.num_folds, group_name);
            if(cur_k > K)
            {
                K = cur_k;
            }
        }
    }

    std::cout << "Created " << K << " folds for experiment " 
              << out_dp << std::endl;

    return EXIT_SUCCESS;
}

