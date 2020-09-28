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

/* $Id: cross_validate.cpp 25834 2020-09-28 17:23:27Z kobus $ */

#include <cstdlib>
#include "dbn_cpp/cross_validate_util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/time_util.h"

#include <ergo_cpp/mh.h>
#include <ergo_cpp/record.h>

#include "dbn_cpp/data.h"
#include "dbn_cpp/real_data_parser.h"

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


        // check if the data has already been parsed
        if(exp.data.data_dp != "")
        {
            // make sure the list is also provided 
            IFT(!exp.data.id_list_fp.empty(), Illegal_argument, 
                "must provide the list id since the data directory is provided.");
            // make sure the grouping-variable is not provided 
            IFTD(exp.data.grouping_var == "", Illegal_argument, 
                "the data needs to be reprocessed to groups " 
                "based on grouping-variable %s", (exp.data.grouping_var.c_str()));
        }
        else
        {
            string outdir_base = exp.out_dp;
            string data_dir = exp.out_dp + "../data/";
            // hack to make Condor respect the output
            ETX(ivi_c::ivi_mkdir(outdir_base.c_str()));
            ofstream touch((outdir_base + "/invocation.txt").c_str());
            touch << "Program invocation:\n" << argv[0] << ' ' << ' '
                  << outdir_base << '\n';
            string output_dir("");
            if(exp.data.grouping_var == "")
            {
                output_dir = data_dir;
            }
            Group_map group_map;
            vector<Data> all_data = parse_real_data(
                                            exp.lss_set.moderators, 
                                            exp.likelihood.obs_names, 
                                            exp.data.csv_fp, 
                                            exp.data.distinguisher,
                                            group_map,
                                            exp.data.grouping_var,
                                            exp.lss.num_oscillators);
            write_data(all_data, output_dir);
            write_group(group_map, output_dir);
            std::cout << "Finished reading in data from the .csv file\n";
        }

        // run cross fold validation 

        struct timespec begin, finish;
        long long start, elapsed; 
        const double NANOS = 1e9;
        get_current_time(&begin);
        start = begin.tv_sec * NANOS + begin.tv_nsec;

        run_cross_validate(exp);

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
