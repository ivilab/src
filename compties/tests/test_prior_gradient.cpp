/* =========================================================================== *
   |
   |  Copyright (c) 1994-2011 by Kobus Barnard (author)
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
   |  Author: Jinyan Guan 
 * =========================================================================== */

/* $Id: test_prior_gradient.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <l/l_sys_time.h>
#include <l_cpp/l_test.h>
#include <diff_cpp/diff_gradient.h>
#include <diff_cpp/diff_gradient_mt.h>
#include <vector>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "dbn_cpp/prior.h"
#include "dbn_cpp/posterior.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/adapter.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"

#define VERBOSE 0

using namespace std;
using namespace ivi;
using namespace ivi::ties;

Data_options data_opt;
Lss_options lss_opt;
Lss_set_options lss_set_opt;
Prior_options prior_opt;
Likelihood_options ll_opt;
size_t num_threads;

/** @brief options handler  */
void process_options(int argc, const char** argv);

void reset(Lss_set& lss_set)
{
    BOOST_FOREACH(Linear_state_space& lss, lss_set.lss_vec())
    {
        lss.changed_index() = 0;
    }
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
        process_options(argc, argv);
        cout << " Start reading data ...\n";

        vector<Data> data_all; 
        vector<size_t> id_lists = parse_list(data_opt.id_list_fp);
        boost::format data_fmt(data_opt.data_dp + "/%04d.txt");
        for(size_t i = 0; i < id_lists.size(); i++)
        {
            string fname = (data_fmt % id_lists[i]).str();
            Data data = parse_data(fname);
            data_all.push_back(data);
        }

        cout << " Creating Lss_set ... \n";
        // Creat Lss_set
        State_vec init_states(data_all.size());
        Double_vv times_all;
        times_all.reserve(data_all.size());
        for(size_t i = 0; i < data_all.size(); i++)
        {
            // create Linear_state_space 
            const Data& data = data_all[i];
            const Double_v& times = data.times;
            times_all.push_back(times);

            // init state
            State_type init_state = estimate_init_states(
                                           data, 
                                           ll_opt.training_percent,
                                           ll_opt.obs_names[0]);
        }

        // create lss_set
        std::vector<double> clo_variances;
        Double_v gp_scales;
        Double_v gp_sigvars;

        //size_t n_p = prior_opt.prior_indices.size();
        if(!prior_opt.fixed_clo)
        {
            size_t n_p = param_length(lss_opt.num_oscillators);
            double sigma = prior_opt.clo_sigma;
            clo_variances.resize(n_p, sigma * sigma);
            if(lss_opt.drift)
            {
                gp_scales.resize(n_p, prior_opt.gp_scale);
                gp_sigvars.resize(n_p, prior_opt.clo_sigma * prior_opt.clo_sigma);
            }
        }

        Lss_set lss_set(id_lists,
                        lss_set_opt.get_all_moderators(
                            lss_opt.ignore_clo,
                            lss_opt.num_oscillators,
                            lss_opt.num_params),
                        init_states,
                        data_all,
                        prior_opt.gp_scale,
                        prior_opt.clo_sigma,
                        prior_opt.poly_sigma,
                        prior_opt.outcome_sigma,
                        ll_opt.obs_names);


        cout << " Creating priorr ... \n";

        size_t num_oscs = lss_opt.num_oscillators;
        size_t num_params = param_length(num_oscs);

        const double default_mod_coef_prior_sigma = 100.0;
        Independent_blr_prior lss_mean_var_prior(
                            std::vector<double>(
                                lss_set.clo_param_size(), 
                                prior_opt.clo_sigma_shape), 
                            std::vector<double>(
                                lss_set.clo_param_size(), 
                                prior_opt.clo_sigma_scale),
                            lss_set.pred_coefs(),
                            get_coef_covariance(lss_set.pred_coefs(), 
                               default_mod_coef_prior_sigma),
                            lss_set.get_X_ts(),
                            lss_set.get_X_t_Xs(),
                            prior_opt.variance_prior_type);

        Shared_scale_prior lss_scale_prior(//prior_opt.prior_indices,
                            std::vector<double>(
                                 num_params,
                                 prior_opt.gp_scale_shape), 
                            std::vector<double>(
                                 num_params, 
                                 prior_opt.gp_scale_scale));

        Shared_noise_prior obs_prior(std::vector<double>(ll_opt.obs_names.size(), 
                                             prior_opt.obs_noise_shape),
                                     std::vector<double>(ll_opt.obs_names.size(), 
                                             prior_opt.obs_noise_scale));
        Cluster_prior cluster_prior;
        Shared_lss_prior prior(lss_mean_var_prior, lss_scale_prior, cluster_prior);
        Independent_blr_posterior posterior(lss_mean_var_prior);

        Shared_param_adapter lss_adapter;

        cout << " Start testing shared_param gradient ...\n";
        const double shared_grad_step = 0.01;
        vector<double> grad_steps(lss_adapter.size(&lss_set), shared_grad_step);

        Shared_gradient<Independent_blr_posterior, Shared_param_adapter> 
                                                 grad(posterior, 
                                                     lss_adapter, 
                                                     grad_steps, 
                                                     false);

        Shared_gradient_mt<Independent_blr_posterior, Shared_param_adapter> 
                                                    grad_mt(posterior, 
                                                           lss_adapter,
                                                           grad_steps, 
                                                           false,
                                                           num_threads);


        vector<double> g = grad(lss_set);
        reset(lss_set);
        Vector G(g.begin(), g.end());

        const double epsilon = 1e-5;
        const size_t N = 100;
        long t = 0;
        std::cout << "=========== SERIAL  =======================\n";
        for(size_t i = 0; i < N; i++)
        {
            ivi_c::init_real_time();
            vector<double> g1 = grad(lss_set);
            t += ivi_c::get_real_time();  

            Vector G1(g1.begin(), g1.end());
            double diff = vector_distance(G1, G);
            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
            }
            TEST_TRUE(diff < epsilon);
            reset(lss_set);
        }
        double sgt = t / 1000.0;

        std::cout << "=========== MT SERIAL gradient =================\n";
        t = 0;
        for(size_t i = 0; i < N; i++)
        {
            ivi_c::init_real_time();
            vector<double> g_mt = grad_mt(lss_set);
            t += ivi_c::get_real_time();  

            Vector G_mt(g_mt.begin(), g_mt.end());
            double diff = vector_distance(G, G_mt);

            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
            }
            TEST_TRUE(diff < epsilon);
            reset(lss_set);
        }
        double msgt = t / 1000.0;

        cout << "Average Time for serial gradient: " << sgt/N << endl;
        cout << "Average Time for MT gradient: " << msgt/N << endl;

        RETURN_VICTORIOUSLY();
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

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

namespace po = boost::program_options;
void process_options(int argc, const char** argv)
{
    po::variables_map vm;

    try
    {
        po::options_description hidden_options("Hidden options");
        hidden_options.add_options()
            ("num-threads,T",
                po::value<size_t>(&num_threads)->default_value(1),
                "the number of threads in gradient computation");

        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("config-file,C",
                po::value<string>(),
                "path of file containing program options in opt=val format");

        bpo::options_description data_options = make_options(data_opt);
        bpo::options_description lss_options = make_options(lss_opt);
        bpo::options_description lss_set_options = make_options(lss_set_opt);
        bpo::options_description prior_options = make_options(prior_opt);
        bpo::options_description ll_options = make_options(ll_opt);

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options)
                       .add(data_options)
                       .add(lss_options)
                       .add(lss_set_options)
                       .add(prior_options)
                       .add(ll_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("num-threads", 1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);
        // help called?
        if(vm.count("help"))
        {
            cout << "usage: ./hmc data-dir"
                      << visible_options << "\n";

            exit(EXIT_SUCCESS);
        }


        // open config file if it is given
        if(vm.count("config-file"))
        {
            string config_file_name = vm["config-file"].as<string>();
            ifstream config_fs(config_file_name.c_str());
            IFTD(config_fs.is_open(), IO_error, 
                    "Error opening configuration file: %s.",
                    (config_file_name.c_str()));

            po::store(po::parse_config_file(config_fs, visible_options), vm);
        }

        po::notify(vm);
    }
    catch(const po::error& err)
    {
        throw ivi::Exception(err.what());
    }    
    catch(const exception& ex)
    {
        throw ex;
    }    
}
