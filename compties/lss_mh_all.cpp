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

/* $Id: lss_mh_all.cpp 25834 2020-09-28 17:23:27Z kobus $ */

#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <l_cpp/l_exception.h>
#include <gp_cpp/gp_mean.h>

#include <ergo_cpp/mh.h>
#include <ergo_cpp/record.h>

#include <iostream>

#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>
#include <boost/format.hpp>

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
using namespace ivi;
using namespace ivi::ties;
using namespace std;

/** @brief  Main, baby! */
int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif

    try
    {
        Ties_experiment exp = experiment_from_cl_options(argc, argv);

        // random seed
        seed_sampling_rand(exp.rand_seed);
        ivi_c::ivi_seed_rand_2(exp.rand_seed);
        srand(exp.rand_seed);
        ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);

        lss_mh_fitting(exp);
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

