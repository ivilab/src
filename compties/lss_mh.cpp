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

/* $Id: lss_mh.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <l_cpp/l_exception.h>
#include <gp_cpp/gp_mean.h>

#include <ergo/mh.h>
#include <ergo/record.h>

#include <iostream>

#ifdef KJB_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/adapter.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/posterior.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/base_line_models.h"

using namespace ergo;
using namespace kjb;
using namespace kjb::ties;
using namespace std;

/** @brief  Main, baby! */
int main(int argc, const char** argv)
{
#ifdef TEST
    kjb_c::kjb_init();
    kjb_c::kjb_l_set("heap-checking", "off");
    kjb_c::kjb_l_set("initialization-checking", "off");
#endif

    try
    {
        // process options
        Ties_experiment exp = experiment_from_cl_options(argc, argv);

        // random seed
        seed_sampling_rand(exp.rand_seed);
        kjb_c::kjb_seed_rand_2(exp.rand_seed);
        srand(exp.rand_seed);
        ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);

        // Read in data
        if(!kjb_c::is_file(exp.data.data_fname.c_str()))
        {
            KJB_THROW_2(Illegal_argument, "-F option specified filename '"
                          + exp.data.data_fname + "' which is not a regular "
                          + "file, as required.\n");
        }
        cout << "Read in data ..." << endl;
        Data data = parse_data(exp.data.data_fname);

        Vector errors;
        Vector sampled_errors;
        std::vector<Vector> obs_errors;
        std::vector<Vector> obs_sampled_errors;
        std::cout << "Start running MH for individual CLO model \n";
        lss_mh_fitting(data, exp, 
                       errors, sampled_errors, 
                       obs_errors, obs_sampled_errors);
        std::cout << "Finished running MH for individual CLO model \n";

    }
    catch(Option_exception& e)
    {
        cerr << e.get_msg() << endl;
        exit(1);
    }
    catch(IO_error& e)
    {
        cerr << e.get_msg() << endl;
        exit(1);
    }
    catch(Exception& e)
    {
        cerr << "Uncaught exception: " << e.get_msg() << endl;
        exit(1);
    }
    catch(exception& e)
    {
        cerr << "Uncaught exception: " << e.what() << endl;
        exit(1);
    }
    catch(boost::exception& e)
    {
        cerr << "Uncaught exception: "
            << boost::diagnostic_information(e) << endl;
        exit(1);
    }
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }

    return EXIT_SUCCESS;
}

