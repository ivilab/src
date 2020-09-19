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

#include <ostream>
#include <string>

#include <ergo/mh.h>
#include <ergo/record.h>

#include "dbn_cpp/base_line_models.h"
#include "dbn_cpp/cross_validate_util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/real_data_parser.h"
#include "dbn_cpp/util.h"

using namespace ivi;
using namespace ivi::ties;

int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif

    // Get experiment options 
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
    std::string outdir_base = exp.out_dp;
    std::string data_dir = exp.out_dp + "/data/";

    // preset some options  
    exp.run.estimate_grad_step = true;
    //exp.run.shared_sample_approach="gibbs";
    //exp.fold_thrd = false;
    //exp.not_record = true;
    
    // parse in the real data 
    std::vector<Data> all_data;
    if(exp.data.csv_fp != "")
    {
        Group_map group_map;
        all_data = parse_real_data(exp.lss_set.moderators, 
                                        exp.likelihood.obs_names, 
                                        exp.data.csv_fp, 
                                        exp.data.distinguisher,
                                        group_map,
                                        exp.data.grouping_var,
                                        exp.lss.num_oscillators);
        if(exp.data.average_size > 1)
        {
            BOOST_FOREACH(Data& data, all_data)
            {
                size_t time_length = data.times.size();
                size_t new_length = 1.0 * time_length / exp.data.average_size;
                data.times.resize(new_length);
                BOOST_FOREACH(Obs_map::value_type& value, data.observables)
                {
                    // copy the old values
                    Vector_v obs_vals = value.second;
                    for(size_t i = 0; i < obs_vals.size(); i++)
                    {
                        value.second[i].resize(new_length);
                        size_t cur_index = 0;
                        size_t new_index = 0;
                        while(cur_index < obs_vals[i].size())
                        {
                            size_t valid = 0;
                            double ave_value = 0.0;
                            for(size_t j = cur_index; j < cur_index + exp.data.average_size; j++)
                            {
                                if(!invalid_data(obs_vals[i][j]))
                                {
                                    ave_value += obs_vals[i][j];
                                    valid++;
                                }
                            }
                            if(valid > 0) ave_value /= valid;
                            else ave_value = INT_MAX;
                            cur_index += exp.data.average_size;
                            value.second[i][new_index++] = ave_value;
                        }
                    }
                }
            }
        }

        if(exp.data.standardize_data)
        {
            BOOST_FOREACH(const std::string& obs, exp.likelihood.obs_names)
            {
                BOOST_FOREACH(Data& data, all_data)
                {
                    std::pair<Vector, Vector> stats = 
                            standardize(data, obs, true, true);
                    std::cout << obs << ": " << " mean: " << stats.first 
                                             << " variance: " << stats.second
                                             << std::endl;
                }
            }
            
        }
        write_data(all_data, data_dir);
        write_group(group_map, data_dir);
    }
    else
    {
        IFT(exp.data.data_dp != "" && !exp.data.id_list_fp.empty(), 
                Illegal_argument, "Please provide the data-dir and id-list-fp");
        std::vector<size_t> ids = parse_list(exp.data.id_list_fp);
        boost::format data_fmt(exp.data.data_dp + "/%04d.txt");
        for(size_t i = 0; i < ids.size(); i++)
        {
            std::string fname = (data_fmt % ids[i]).str();
            all_data.push_back(parse_data(fname));
            // check to see if obs_names is in the data file
            BOOST_FOREACH(const std::string& obs_name, exp.likelihood.obs_names)
            {
                if(all_data.back().observables.find(obs_name) 
                        == all_data.back().observables.end())
                {
                    IVI_THROW_3(Illegal_argument,
                            "the observable %s is not provided in the data file.",
                            (obs_name.c_str()));
                }
            }
        }
    }

    // hack to make Condor respect the output
    ETX(ivi_c::ivi_mkdir(outdir_base.c_str()));
    std::ofstream touch((outdir_base + DIR_STR + "invocation.txt").c_str());
    touch << "Program invocation:\n" << argv[0] << ' ' << ' '
          << outdir_base << '\n';

    std::cout << " training percent; " << exp.likelihood.training_percent << std::endl;
    // run average model 
    for(size_t i = 0; i < exp.likelihood.obs_names.size(); i++)
    {
        // average models
        std::cout << "average for observable " << exp.likelihood.obs_names[i] << std::endl;
        exp.out_dp = outdir_base + "/average/" + exp.likelihood.obs_names[i] + "/"; 
        average_model_fitting(all_data, 
                              exp.out_dp, 
                              exp.likelihood.obs_names[i],
                              exp.data.distinguisher,
                              exp.likelihood.training_percent,
                              exp.likelihood.obs_names.size());
    }

    // run line model 
    for(size_t i = 0; i < exp.likelihood.obs_names.size(); i++)
    {
        // line fitting models
        std::cout << "line fitting for observable " << exp.likelihood.obs_names[i] << std::endl;
        exp.out_dp = outdir_base + "/line/" + exp.likelihood.obs_names[i] + "/"; 
        line_model_fitting(all_data,
                           exp.out_dp,
                           exp.likelihood.obs_names[i],
                           exp.data.distinguisher,
                           exp.likelihood.training_percent,
                           exp.likelihood.obs_names.size());

    }

/*
 * This block is disabled. We are now using run_fold to fit individual CLO
 * models since we need to fit the observation nosie variance.
 * 20 Sept, 2015 --- Jinyan 
 
    // individual CLO models (couples are fitting independently)
    std::cout << "fitting coupled oscillator for echo couple independently" << std::endl;
    exp.out_dp = outdir_base + "/individual-CLO/";
    lss_mh_fitting(all_data, exp);
*/

/*
 * This block is disabled so we can do the folds separately using run_fold instead.
 * 6 April 2015, Andrew Predoehl
 *
 * 15 May 2015 -- temporarily restored by Jinyan Guan
 * 4 June 2015 -- again disabled by Andrew Predoehl; list file creation also
 *                omitted.  This is considered to be a permanent change, so
 *                that run_base_models only does the "easy" baselines, not
 *                the shared-param-CLO model, which is hard.
 */
#if 0
    // create the list file for cross-validate 
    std::string id_list_fp(outdir_base + "/ids.txt");
    std::ofstream id_ofs(id_list_fp.c_str());
    BOOST_FOREACH(const Data& data, all_data)
    {
        id_ofs << data.dyid << std::endl;
    }
    exp.data.id_list_fp = id_list_fp;
    exp.data.data_dp = data_dir;

    // cross validation of a specified model
    std::cout << "running cross validation on couple shared parameteres" << std::endl;
    exp.out_dp = outdir_base + "/shared-param-CLO/";
    run_cross_validate(exp);
#endif

    return EXIT_SUCCESS;
}

