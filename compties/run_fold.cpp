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

/* $Id: run_fold.cpp 25834 2020-09-28 17:23:27Z kobus $ */

#include <l_cpp/l_filesystem.h>
#include <l/l_sys_time.h>

#include "dbn_cpp/data.h"
#include "dbn_cpp/likelihood.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/lss_set_sampler.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/cross_validate_util.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/random.hpp>
#include <boost/thread.hpp>

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
    try
    {
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

        // set random seed 
        ivi_c::ivi_seed_rand(exp.rand_seed, exp.rand_seed);
        ivi_c::ivi_seed_rand_2(exp.rand_seed);
        ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);
        srand(exp.rand_seed);
      
        if(exp.fold_info_dp == "")
        {
            exp.fold_info_dp = exp.out_dp;
        }

        string fold_fmt(exp.fold_info_dp + "/fold-%02d");
        std::vector<std::string> fold_dirs = dir_names_from_format(fold_fmt);
        size_t K = fold_dirs.size();
        vector<Vector> training_errors(K);
        vector<Vector> testing_errors(K);
        vector<Vector> sample_errors(K);
        std::vector<std::vector<Vector> > obs_training_errors(K);
        std::vector<std::vector<Vector> > obs_testing_errors(K);
        std::vector<std::vector<Vector> > obs_sample_errors(K);

        struct timespec begin, finish;
        long long start, elapsed; 
        const double NANOS = 1e9;
        get_current_time(&begin);
        start = begin.tv_sec * NANOS + begin.tv_nsec;
        fold_worker(exp.fold, 
                    training_errors, 
                    testing_errors, 
                    sample_errors, 
                    obs_training_errors, 
                    obs_testing_errors, 
                    obs_sample_errors, 
                    exp);
        // visualize 
        boost::format out_fmt(exp.out_dp + "/fold-%02d/");
        string out_dp = (out_fmt % exp.fold).str();

        if(exp.data.grouping_var == "")
        {
            string list_fp = out_dp + "/training-list.txt";
            string lss_dir = out_dp + "/training/";
            plot_exp_dir(lss_dir, list_fp, exp.data.data_dp);
            list_fp = out_dp + "/testing-list.txt";
            lss_dir = out_dp + "/testing/";
            plot_exp_dir(lss_dir, list_fp, exp.data.data_dp);
        }
        else
        {
            boost::format train_fmt(out_dp + "/" 
                    + exp.data.grouping_var + "-%d-training-list.txt");
            boost::format test_fmt(out_dp + "/"
                    + exp.data.grouping_var + "-%d-testing-list.txt");
            std::vector<std::string> data_groups = get_data_group_dirs(
                                                        exp.data.data_dp);
            size_t num_groups = data_groups.size();
            for(size_t g = 0; g < num_groups; g++)
            {
                size_t found = data_groups[g].find_last_of("/\\");
                std::string group_name = data_groups[g].substr(found+1);

                std::string list_fp = out_dp + "/" + group_name + "-training-list.txt";
                string lss_dir = out_dp + "/training/" + group_name + "/";
                plot_exp_dir(lss_dir, list_fp, data_groups[g]);

                list_fp = out_dp + "/" + group_name + "-testing-list.txt";
                lss_dir = out_dp + "/testing/" + group_name + "/";
                plot_exp_dir(lss_dir, list_fp, data_groups[g]);
            }
        }

        get_current_time(&finish);
        elapsed = finish.tv_sec * NANOS + finish.tv_nsec - start;
        double time = elapsed / 1e9; 
        std::cout << "Running time for " << argv[0] << " is: " 
                  << time/(60.0 * 60.0) << " (h)" << std::endl;
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
        cerr << e.get_msg() << endl;
        exit(1);
    }
    catch(exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    catch(boost::exception& e)
    {
        cerr << boost::diagnostic_information(e) << endl;
        exit(1);
    }
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }

    std::cout << "Successful exit from " << argv[0] << '\n';
    return EXIT_SUCCESS;
}
