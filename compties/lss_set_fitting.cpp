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

/* $Id: lss_set_fitting.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include "dbn_cpp/data.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/lss_set_sampler.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"

#include <string>
#include <vector>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#ifndef IVI_HAVE_ERGO
#error "You need libergo to use this program"
#endif

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
    exp.run.iter_log_fname = exp.out_dp + "/log.txt";

    // set random seed 
    seed_sampling_rand(exp.rand_seed);
    ivi_c::ivi_seed_rand(exp.rand_seed, exp.rand_seed);
    ivi_c::ivi_seed_rand_2(exp.rand_seed);
    ergo::global_rng<boost::mt19937>().seed(exp.rand_seed);

    cout << " Initilizing ...\n";
    // check the grouping_var
    
    size_t num_data_groups = exp.cluster.num_groups;
    std::vector<std::string> data_groups = get_data_group_dirs(exp.data.data_dp);
    if(exp.cluster.num_groups == 1 && exp.data.grouping_var != "")
    {
        num_data_groups = data_groups.size();
        exp.cluster.num_groups = num_data_groups;
    }
    if(num_data_groups > 1)
    {
        string list_fp = "ids.txt";
        std::vector<std::string> list_fps(num_data_groups);
        for(size_t g = 0; g < num_data_groups; g++)
        {
            size_t found = data_groups[g].find_last_of("/\\");
            std::string group_name = data_groups[g].substr(found+1);
            std::string fp = exp.data.data_dp + "/" + group_name + "/" + list_fp;
            list_fps[g] = fp;
        }
        exp.data.id_list_fp = list_fps;
    }
    else if(exp.data.id_list_fp.empty())
    {
        std::string fp = exp.data.data_dp + "/ids.txt";
        exp.data.id_list_fp.push_back(fp);
    }
   
    Lss_set_sampler lss_set_sampler(exp);
    cout << " Start to run ...\n";
    Lss_set best_lss_set(lss_set_sampler.lss_set());

    //exp.likelihood.training_percent = 1.0;
    //std::cout << "setting training percent to 1.0\n";

    best_lss_set = lss_set_sampler.train_model(exp.train_num_iterations);
    cout << " Write results ...\n";
    best_lss_set.write(exp.out_dp);


    cout << " Create plotting data ...\n";
    const std::vector<Data>& data = lss_set_sampler.data();

    string char_out_dir(exp.out_dp + "/characteristic_models/");
    ETX(ivi_c::ivi_mkdir(char_out_dir.c_str()));

    // get the moderator reprsentative
    vector<vector<pair<double, double> > > mods 
        = get_percentile_moderator(data, exp.lss_set.moderators, 
                exp.lss.num_oscillators, exp.lss_set.shared_moderator);

    size_t num_mod = mods.size();
    size_t num_mod_strs = exp.lss_set.moderators.size();

    for(size_t i = 0; i < mods.size(); i++)
    {
        for(size_t j = 0; j < mods[i].size(); j++)
        {
            std::cout << mods[i][j].first << " " << mods[i][j].second << "; ";
        }
        std::cout << std::endl;
    }

    // initial state dimension
    size_t inD = exp.lss.num_oscillators * 2;
    // get the observation state based on the first observable
    std::vector<Vector> lsf_coefs = get_init_state_least_square_fitting_coef(
            data, 
            exp.likelihood.training_percent,
            exp.lss_set.moderators,
            exp.likelihood.obs_names[0],
            exp.lss_set.shared_moderator,
            exp.lss.num_oscillators);
    // output the coefs (DEBUG, delete later)
    //std::copy(lsf_coefs.begin(), lsf_coefs.end(), ostream_iterator<Vector>(cout, "\n"));

    boost::format sub_fmt(char_out_dir + "/%02d/");
    for(size_t i = 0; i < mods.size(); i++)
    {
        string sub_dir = (sub_fmt % i).str();
        ETX(ivi_c::ivi_mkdir(sub_dir.c_str()));
        string info_fp = sub_dir + "/moderator_info.txt";
        ofstream info_ofs(info_fp.c_str());
        IFTD(info_ofs.is_open(), IO_error, "can't open file %s", 
                (info_fp.c_str()));
        if(exp.lss_set.shared_moderator)
        {
            BOOST_FOREACH(const std::string& mod, exp.lss_set.moderators)
            {
                info_ofs << setw(25) << right << mod;
            }
        }
        else
        {
            BOOST_FOREACH(const std::string& mod, exp.lss_set.moderators)
            {
                for(int j = 0; j < exp.lss.num_oscillators; j++)
                {
                    std::string str(mod + "-" + exp.data.distinguisher + "-" 
                            + boost::lexical_cast<string>(j));
                    info_ofs << setw(25) << right << str << " ";
                }
            }
        }
        info_ofs << std::endl;

        Vector cur_mod((int)mods[i].size() + 1, 1.0);
        for(size_t j = 0; j < mods[i].size(); j++)
        {
            info_ofs << setw(25) << right << mods[i][j].first;
            cur_mod[j + 1] = mods[i][j].second;
        }
        info_ofs << std::endl;

        for(size_t j = 0; j < mods[i].size(); j++)
        {
            info_ofs << setw(25) << right << mods[i][j].second;
        }
        // construct the moderator map
        Mod_map mod_rep;
        size_t k = 0;
        BOOST_FOREACH(const std::string& mod, exp.lss_set.moderators)
        {
            mod_rep[mod] = Double_v((int)exp.lss.num_oscillators, 0.0);
            for(size_t j = 0; j < exp.lss.num_oscillators; j++)
            {
                size_t j_index = exp.lss_set.shared_moderator ? k : j;
                mod_rep[mod][j] = mods[i][j_index].second;
            }
            k++;
        }

        // compute the init state based on the moderators
        State_type init_state(inD);
        if(mods.size() > 1)
        {
            for(size_t j = 0; j < inD; j++)
            {
                init_state[j] = dot(lsf_coefs[j], cur_mod);
            }
        }
        else
        {
            init_state = estimate_average_init_state(
                                                 data, 
                                                 exp.likelihood.training_percent, 
                                                 exp.likelihood.obs_names[0]);
        }

        // initialize the times for the character model 
        Double_v time(exp.character_model_length);
        for(size_t j = 0; j < exp.character_model_length; j++)
        {
            time[j] = j; 
        }

        Double_v com_params = coupled_oscillator_params
                                (exp.lss.num_oscillators,
                                 exp.lss.init_period,
                                 exp.lss.init_damping,
                                 exp.lss.use_modal,
                                 false);
        Vector obs_sigmas((int)exp.likelihood.obs_names.size(), 
                          exp.likelihood.init_noise_sigma);
        Coupled_oscillator_v clos(time.size() - 1, 
                                  Coupled_oscillator(com_params, exp.lss.use_modal));
        Linear_state_space lss(time, 
                               init_state, 
                               clos,
                               exp.likelihood.obs_names,
                               obs_sigmas,
                               exp.lss.polynomial_degree);
        // initialize the predictors based on the characteer model 
        lss.init_predictors(data.front().moderators, 
                exp.lss_set.get_all_moderators(
                    exp.lss.ignore_clo,
                    exp.lss.num_oscillators,
                    exp.lss.num_params));

        // set the observation coefs 
        const std::vector<vector<Vector> >& obs_coefs = best_lss_set.obs_coefs();
        for(size_t j = 0; j < obs_coefs.size(); j++)
        {
            for(size_t k = 0; k < obs_coefs[j].size(); k++)
            {
                lss.set_obs_coef(j, k, obs_coefs[j][k]);
            }
        }

        // update the CLO params 
        const std::vector<Vector>& preds = lss.get_predictors();
        for(size_t g = 0; g < best_lss_set.num_groups(); g++)
        {
            // get the predictors
            const std::vector<Vector>& pred_coefs = best_lss_set.pred_coefs(g);
             
            for(size_t j = 0; j < com_params.size(); j++)
            {
                double val = dot(preds[j], pred_coefs[j]);
                // i is a person-specific index, so there is corresponding mean 
                //if(persons_[i])
                if(!lss.allow_drift())
                {
                    Coupled_oscillator_v& clos = lss.coupled_oscillators();
                    BOOST_FOREACH(Coupled_oscillator& clo, clos)
                    {
                        clo.set_param(j, val);
                    }
                    lss.changed_index() = 0;
                }
                else //if(lss.allow_drift())
                {
                    lss.set_gp_mean(j, gp::Constant(val));
                }
            }

            if(num_data_groups == 1) lss.write(sub_dir);
            else
            {
                size_t found = data_groups[g].find_last_of("/\\");
                std::string group_name = data_groups[g].substr(found+1);
                std::string group_sub_dir = sub_dir + "/" + group_name;
                lss.write(group_sub_dir);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////
    //       compute the testing error
    ///////////////////////////////////////////////////////////////////

    // record the testing errors for each couple 
    ivi_c::init_real_time();
    string test_fp("err_couples.txt");
    const std::vector<Linear_state_space>& lss_set = best_lss_set.lss_vec();
    size_t num_lss = lss_set.size();
    assert(num_lss == data.size());

    const std::vector<std::string>& obs_names = exp.likelihood.obs_names;
    size_t num_obs = obs_names.size();
    std::vector<Vector> testing_errors_temp(num_lss);
    std::vector<std::vector<Vector> > obs_testing_errors_temp(num_obs);

    for(size_t i = 0; i < num_lss; i++)
    {
        std::vector<Vector> obs_errors;
        Vector testing_error = compute_ave_error(data[i], 
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
    //const size_t num_samples = 1;
    const size_t num_samples = 100;
    // indexed by <num_lss><num_samples>
    std::vector<std::vector<Linear_state_space> > all_samples = 
        lss_set_sampler.pred_samples(best_lss_set, num_samples);

    string sampled_err_couple_fp("sampled_err_couples.txt");

    std::vector<Vector> sample_errors_temp(num_lss);
    std::vector<std::vector<Vector> > sp_obs_temp(num_obs);
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
                  obs_names,
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
   
    long st = ivi_c::get_real_time();
    std::cout << "Reporting error takes: " << st / (1000.0) << " s.\n";

    std::cout << "Plot each model and data \n";
    //plot each dyad 
    if(num_data_groups == 1)
    {
        plot_exp_dir(exp.out_dp, exp.data.id_list_fp[0], exp.data.data_dp);
    }
    else
    {
        for(size_t g = 0; g < num_data_groups; g++)
        {
            size_t found = data_groups[g].find_last_of("/\\");
            std::string group_name = data_groups[g].substr(found+1);
            std::string group_dp = exp.out_dp + "/" + group_name;
            plot_exp_dir(group_dp, exp.data.id_list_fp[g], data_groups[g]);
        }
    }

    return EXIT_SUCCESS;
}

