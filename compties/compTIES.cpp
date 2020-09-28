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

/* $Id: cross_validate.cpp 20874 2016-10-06 06:13:05Z jguan1 $ */

#include <cstdlib>

#include <ergo_cpp/mh.h>
#include <ergo_cpp/record.h>

#include "dbn_cpp/cross_validate_util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/time_util.h"
#include "dbn_cpp/base_line_models.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/real_data_parser.h"
#include "dbn_cpp/lss_set_sampler.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

struct timespec begin_time; 
struct timespec finish_time;
long long start, elapsed; 
const double NANOS = 1e9;

void report_time()
{
    get_current_time(&finish_time);
    elapsed = finish_time.tv_sec * NANOS + finish_time.tv_nsec - start;
    double time = elapsed / 1e9; 
    std::cout << "Running time for compTIES is: " 
              << time/(60.0 * 60.0) << " (h)" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif
    try
    {
        // Timing 
        get_current_time(&begin_time);
        start = begin_time.tv_sec * NANOS + begin_time.tv_nsec;

        // Create an experiment based on the options
        Ties_experiment exp = experiment_from_cl_options(argc, argv);

        // set random seed 
        ivi_c::ivi_seed_rand(exp.rand_seed, exp.rand_seed);
        ivi_c::ivi_seed_rand_2(exp.rand_seed);
        ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);
        srand(exp.rand_seed);

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

        // Requied directories inside the exp.out_dp

        std::cout << "data directory: " << exp.data.data_dp << std::endl;
        // Check the data directory
        if(!ivi_c::is_directory(exp.data.data_dp.c_str()))
        {
            std::cerr << "No \"data\" directory inside the experiment directory: \n"
                      << exp.out_dp << "\n"
                      << "Exiting the program ...\n";
            report_time();
            exit(EXIT_SUCCESS);
        }

        if(!exp.run_average_model && !exp.run_line_model && !exp.fit_all_data &&
                !ivi_c::is_directory(exp.fold_info_dp.c_str()))
        {
            std::cerr << "\"folds\" directory " << exp.fold_info_dp << " does not exit."
                      << "Exit the program ...\n";
            report_time();
            exit(EXIT_SUCCESS);
        }

        // Run average model
        if(exp.run_average_model)
        {
            exp.data.id_list_fp.clear();
            exp.data.id_list_fp.push_back(
                        std::string(exp.data.data_dp + "/ids.txt"));
            BOOST_FOREACH(const std::string& obs_name, exp.likelihood.obs_names)
            {
                std::string cur_dir(exp.out_dp + "/" + obs_name);
                average_model_fitting(exp.data.data_dp, 
                                      exp.data.id_list_fp.front(), 
                                      exp.out_dp, obs_name, 
                                      exp.data.distinguisher, 
                                      exp.likelihood.training_percent);
            }
            report_time();
            exit(EXIT_SUCCESS);
        }

        // Run line model
        if(exp.run_line_model)
        {
            exp.data.id_list_fp.clear();
            exp.data.id_list_fp.push_back(
                        std::string(exp.data.data_dp + "/ids.txt"));
            BOOST_FOREACH(const std::string& obs_name, exp.likelihood.obs_names)
            {
                std::string cur_dir(exp.out_dp + "/" + obs_name);
                line_model_fitting(
                    exp.data.data_dp, 
                    exp.data.id_list_fp.front(), 
                    exp.out_dp, obs_name, 
                    exp.data.distinguisher, 
                    exp.likelihood.training_percent
                );
            }
            report_time();
            exit(EXIT_SUCCESS);
        }


        // Use all the data as training data to fit the model
        if(exp.fit_all_data)
        {
            exp.out_dp += "/all_data/";
            if(exp.data.grouping_var != "")
            {
                string list_fp = "ids.txt";
                std::vector<std::string> data_groups = 
                                    get_data_group_dirs(exp.data.data_dp);
                assert(data_groups.size() == exp.cluster.num_groups);
                exp.data.id_list_fp.clear();
                exp.data.id_list_fp.resize(exp.cluster.num_groups);

                for(size_t g = 0; g < exp.cluster.num_groups; g++)
                {
                    std::string fp = data_groups[g] + "/" + list_fp;
                    if(!ivi_c::is_file(fp.c_str()))
                    {
                        std::cerr << "Data directory " << data_groups[g] 
                                  << " does not have a ids.txt file\n";
                        exit(EXIT_SUCCESS);
                    }
                    exp.data.id_list_fp[g] = fp;
                }

            }
            else
            {
                exp.data.id_list_fp.clear();
                exp.data.id_list_fp.push_back(
                            std::string(exp.data.data_dp + "/ids.txt"));
                if(!ivi_c::is_file(exp.data.id_list_fp[0].c_str()))
                {
                    std::cerr << "Data directory " << exp.data.id_list_fp[0] 
                              << " does not have ids.txt file \n";
                    exit(EXIT_SUCCESS);
                }
            }
            string log_fp = exp.out_dp + "/train_log.txt";
            exp.run.iter_log_fname = log_fp;
            exp.run.fit_err_fname = exp.out_dp + "/train_err.txt";
            exp.run.trace_fname = exp.out_dp + "/train_trace.txt";

            std::cout << "setting training percent to 1.0\n";
            exp.likelihood.training_percent = 1.0;
            ETX(ivi_c::ivi_mkdir(exp.out_dp.c_str()));
            Lss_set_sampler lss_set_sampler(exp);
            Lss_set best_lss_set = 
                    lss_set_sampler.train_model(exp.train_num_iterations);
            cout << " Write results ...\n";
            best_lss_set.write(exp.out_dp);

            cout << " Create plotting data ...\n";
            create_characteristic_models(exp, lss_set_sampler.data(), best_lss_set);

            ///////////////////////////////////////////////////////////////////
            //          Compute the testing error for the MAP of theta
            ///////////////////////////////////////////////////////////////////

            string test_fp("err_couples.txt");
            const std::vector<Linear_state_space>& lss_set = 
                                            best_lss_set.lss_vec();
            size_t num_lss = lss_set.size();

            const std::vector<std::string>& obs_names = exp.likelihood.obs_names;
            size_t num_obs = obs_names.size();
            std::vector<Vector> testing_errors_temp(num_lss);
            std::vector<std::vector<Vector> > obs_testing_errors_temp(num_obs);

            for(size_t i = 0; i < num_lss; i++)
            {
                std::vector<Vector> obs_errors;
                Vector testing_error = compute_ave_error(
                                             lss_set_sampler.data()[i], 
                                             lss_set[i], 
                                             obs_errors,
                                             exp.likelihood.training_percent);
                testing_errors_temp[i] = testing_error;
                // observables
                for(size_t j = 0; j < obs_names.size(); j++)
                {
                    obs_testing_errors_temp[j].push_back(obs_errors[j]);
                } 
            }
            const std::vector<size_t>& ids = best_lss_set.ids();
            report_errors(exp.out_dp,
                          test_fp,
                          testing_errors_temp,
                          obs_testing_errors_temp, 
                          obs_names,
                          ids); 

            string test_err_fp(exp.out_dp + "/err_summary.txt");
            std::pair<Vector, Vector> stat = compute_and_record_errors(
                                                        testing_errors_temp, 
                                                        exp.data.distinguisher,
                                                        test_err_fp);
            Vector testing_errors = stat.first;
            std::vector<Vector> obs_testing_errors(num_obs);
            for(size_t i = 0; i < num_obs; i++)
            {
                string obs_fp(exp.out_dp + "/" 
                                + obs_names[i] + "_err_summary.txt");
                std::pair<Vector, Vector> stat = compute_and_record_errors(
                                                    obs_testing_errors_temp[i], 
                                                    exp.data.distinguisher,
                                                    obs_fp);
                obs_testing_errors[i] = stat.first;
            }
            
            ///////////////////////////////////////////////////////////////////
            //          compute the testing error from all samples 
            ///////////////////////////////////////////////////////////////////
            const size_t num_samples = 100;

            // indexed by <num_lss><num_samples>
            std::vector<std::vector<Linear_state_space> > all_samples = 
                lss_set_sampler.pred_samples(best_lss_set, num_samples);

            string sampled_err_couple_fp("sampled_err_couples.txt");

            std::vector<Vector> sample_errors_temp(num_lss);
            std::vector<std::vector<Vector> > sp_obs_temp(num_obs);
            const std::vector<Data>& data = lss_set_sampler.data();
            for(size_t i = 0; i < num_lss; i++)
            {
                std::vector<Vector> obs_errors;
                sample_errors_temp[i] = compute_ave_error(data[i], 
                                                      all_samples[i], 
                                                      obs_errors, 
                                                      exp.likelihood.training_percent);
                for(size_t j = 0; j < obs_names.size(); j++)
                {
                    sp_obs_temp[j].push_back(obs_errors[j]);
                }
            }

            report_errors(exp.out_dp, 
                          sampled_err_couple_fp,
                          sample_errors_temp,
                          sp_obs_temp,
                          exp.likelihood.obs_names,
                          ids);

            string sampled_err_fp(exp.out_dp + "/sampled_err_summary.txt");
            std::pair<Vector, Vector> sample_stat = compute_and_record_errors(
                                                        sample_errors_temp, 
                                                        exp.data.distinguisher,
                                                        sampled_err_fp);
            Vector sample_errors = sample_stat.first;
            std::vector<Vector> obs_sample_errors(num_obs);
            for(size_t i = 0; i < num_obs; i++)
            {
                string obs_fp(exp.out_dp + "/" 
                                + obs_names[i] + "_sampled_err_summary.txt");
                std::pair<Vector, Vector> stat = compute_and_record_errors(
                                                    sp_obs_temp[i], 
                                                    exp.data.distinguisher,
                                                    obs_fp);
                obs_sample_errors[i] = stat.first;
            }

            // compute the outcome error
            best_lss_set.report_outcome_pred_errs(exp.out_dp);
            std::vector<std::string> err_fps(1, 
                    std::string(exp.out_dp + "/outcome_errs.txt"));
            string err_fp = exp.out_dp + "/outcome_pred_errors.txt";
            report_outcome_errors(err_fps, err_fp, exp.data.distinguisher);

            std::vector<std::string> err_ave_fps(1, 
                    std::string(exp.out_dp + "/outcome_errs_ave.txt"));
            string err_ave_fp = exp.out_dp + "/outcome_pred_errors_ave.txt";
            report_outcome_errors(err_ave_fps, err_ave_fp, exp.data.distinguisher);

            std::vector<std::string> err_prior_fps(1, 
                    std::string(exp.out_dp + "/outcome_errs_prior.txt"));
            string err_prior_fp = exp.out_dp + "/outcome_pred_errors_prior.txt";
            report_outcome_errors(err_prior_fps, err_prior_fp, exp.data.distinguisher);

            // compute the outcome error based on the linear fitting
            if(!exp.data.outcomes.empty())
            {
                Line line;
                double err = line_fitting_outcome(best_lss_set, line, 0);
                std::string outcome_fp(exp.out_dp + "/outcome_line.txt");
                std::ofstream ofs(outcome_fp.c_str());
                IFTD(ofs.is_open(), IO_error, "can't open file %s", 
                        (outcome_fp.c_str()));
                ofs << line.get_params() << " " << err << std::endl;
            }

            //plot each dyad 
            size_t num_data_groups = exp.cluster.num_groups;
            if(num_data_groups > 1 && exp.data.grouping_var != "")
            {
                std::vector<std::string> data_groups = 
                                get_data_group_dirs(exp.data.data_dp);
                for(size_t g = 0; g < num_data_groups; g++)
                {
                    size_t found = data_groups[g].find_last_of("/\\");
                    std::string group_name = data_groups[g].substr(found+1);
                    std::string group_dp = exp.out_dp + "/" + group_name;
                    plot_exp_dir(group_dp, exp.data.id_list_fp[g], data_groups[g]);
                }
            }
            else
            {
                plot_exp_dir(exp.out_dp, exp.data.id_list_fp[0], exp.data.data_dp);
            }

            report_time();
            exit(EXIT_SUCCESS);
        }

        // Run cross validation
        run_cross_validate(exp);

        report_time();
        exit(EXIT_SUCCESS);
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
