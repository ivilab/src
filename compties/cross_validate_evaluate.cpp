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

/* $Id: cross_validate_evaluate.cpp 22569 2019-06-09 18:17:28Z manujinda $ */

#ifndef KJB_HAVE_ERGO
#error "You need libergo to use this program"
#endif

#include <l_cpp/l_filesystem.h>

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


using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
#ifdef TEST
    kjb_c::kjb_init();
    kjb_c::kjb_l_set("heap-checking", "off");
    kjb_c::kjb_l_set("initialization-checking", "off");
#endif
    Ties_experiment exp = experiment_from_cl_options(argc, argv);

    // check the distinguisher
    if(exp.data.distinguisher == "")
    {
        std::cerr << "No distinguisher is provided. Exiting the program ...\n";
        exit(EXIT_SUCCESS);
    }
    if(exp.likelihood.obs_names.empty())
    {
        std::cerr << "No observable is provided. Exiting the program ...\n";
        exit(EXIT_SUCCESS);
    }

    ////////////////////////////////////////////////////////
    //                  COMPUTE THE ERRORS 
    ////////////////////////////////////////////////////////
    string fold_fmt(exp.out_dp + "/fold-%02d/");
    std::vector<std::string> fold_dirs = dir_names_from_format(fold_fmt);
    size_t K = fold_dirs.size();

    std::string err_dp = exp.out_dp + "/errors/";
    ETX(kjb_c::kjb_mkdir(err_dp.c_str()));

    // for all the observables 
    const std::vector<std::string>& obs_names = exp.likelihood.obs_names;
    size_t num_obs = obs_names.size();
    generate_model_name(exp);
    for(size_t i = 0; i < num_obs; i++)
    {
        report_cross_validate_errors(err_dp, 
                                     obs_names[i], 
                                     exp.data.distinguisher,
                                     fold_dirs, 
                                     K,
                                     exp.write_latex_error);
    }
    // report the mean of each obs
    report_cross_validate_errors(err_dp, 
                                 string(""), 
                                 exp.data.distinguisher,
                                 fold_dirs, 
                                 K,
                                 exp.write_latex_error);

    return EXIT_SUCCESS;
}

