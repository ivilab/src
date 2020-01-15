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

/* $Id: test_pos_gradient.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include <l/l_sys_time.h>
#include <l_cpp/l_test.h>
#include <diff_cpp/diff_gradient.h>
#include <diff_cpp/diff_gradient_mt.h>
#include <vector>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "dbn_cpp/options.h"
#include "dbn_cpp/posterior.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/adapter.h"
#include "dbn_cpp/likelihood.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/typedefs.h"

#define VERBOSE 1

using namespace std;
using namespace kjb;
using namespace kjb::ties;

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
    lss_set.update_variances();
    lss_set.update_means();
}

int main(int argc, const char** argv)
{
#ifdef TEST
    //kjb_c::kjb_init();
    //kjb_c::kjb_l_set("heap-checking", "off");
    //kjb_c::kjb_l_set("initialization-checking", "off");
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
            //size_t length = data.times.size();
            const Double_v& times = data.times;
            times_all.push_back(times);

            // init state
            init_states[i] = estimate_init_states(
                                           data, 
                                           ll_opt.training_percent,
                                           ll_opt.obs_names.front());

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

        cout << " Creating posterior ... \n";
        std::vector<Posterior> posteriors;
        std::vector<Likelihood> likelihoods;
        int num_noise = ll_opt.obs_names.size();
        Init_prior init_prior;
        for(size_t i = 0; i < data_all.size(); i++)
        {
            const Data& data = data_all[i];
            Likelihood likelihood(data);
            likelihoods.push_back(likelihood);
            posteriors.push_back(Posterior(likelihood, 
                                           init_prior));
        }

        size_t nosc = lss_opt.num_oscillators;
        size_t num_params = param_length(nosc);

        Shared_noise_prior obs_prior(std::vector<double>(ll_opt.obs_names.size(), 
                                             prior_opt.obs_noise_shape),
                                     std::vector<double>(ll_opt.obs_names.size(), 
                                             prior_opt.obs_noise_scale));
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

        bool learn_clo = true;
        //bool learn_sigma = prior_opt.fixed_clo? false : true;
        Cluster_prior regime_prior;
        Shared_lss_prior shared_prior(lss_mean_var_prior,
                                  lss_scale_prior,
                                  regime_prior,
                                  learn_clo);

        Lss_set_posterior posterior(shared_prior, obs_prior, posteriors); 
        Lss_set_posterior_mt posterior_mt(shared_prior, obs_prior, 
                                          posteriors, num_threads);
        //posterior.set_use_clo_hyper_prior(false);
        //posterior.set_use_noise_prior(false);
        //posterior_mt.set_use_clo_hyper_prior(false);
        //posterior_mt.set_use_noise_prior(false);

        // TEST POSTERIOR 
        cout << " Start testing posterior ... \n";
        long st = 0;
        long pt = 0;
        const double epsilon = 1e-5;
        const size_t N = 10;
        double fac = 1.0/(N*1000.0);
        double ll = 0.0;
        for(size_t i = 0; i < N; i++)
        {
            kjb_c::init_real_time();
            ll = posterior(lss_set);
            st += kjb_c::get_real_time();
            reset(lss_set);
        }
        cout << " SERIAL POSTERIOR: " << st * fac << endl;
        //for(size_t j = 2; j < 22; j++)
        //{
            pt = 0;
            for(size_t i = 0; i < N; i++)
            {
                //Lss_set_posterior_mt posterior_mt(shared_prior, posteriors, j);
                kjb_c::init_real_time();
                double ll_mt = posterior_mt(lss_set);
                pt += kjb_c::get_real_time();
                TEST_TRUE(fabs(ll - ll_mt) < FLT_EPSILON);
                if(VERBOSE)
                {
                    std::cout << ll << " vs " << ll_mt << std::endl;
                }
                reset(lss_set);
            }
            //cout << " " << j << " THREADS PARALL POSTERIOR: " << pt * fac << endl;
            cout << " THREADS PARALL POSTERIOR: " << pt * fac << endl;
        //}
        reset(lss_set);

        //bool exclude_variances = !learn_sigma ? true : false;
        Shared_param_adapter lss_adapter;

        cout << " Start testing shared_param gradient ...\n";
        const double shared_grad_step = 0.001;
        vector<double> grad_steps(lss_adapter.size(&lss_set), shared_grad_step);

        Shared_gradient<Lss_set_posterior, Shared_param_adapter> gradient(
                                                                posterior, 
                                                                lss_adapter,
                                                                grad_steps, 
                                                                false);

        size_t avail_num_threads = boost::thread::hardware_concurrency(); 
        std::cout << "avail num-threads: " << avail_num_threads << std::endl;

        Shared_gradient_mt<Lss_set_posterior, Shared_param_adapter> 
                                              gradient_mt(posterior, 
                                                          lss_adapter,
                                                          grad_steps, 
                                                          false,
                                                          num_threads);

        Shared_gradient<Lss_set_posterior_mt, Shared_param_adapter> 
                                              gradient_pmt(posterior_mt, 
                                                           lss_adapter,
                                                           grad_steps, 
                                                           false);

        Shared_gradient_mt<Lss_set_posterior_mt, Shared_param_adapter> 
                                                gradient_mmt(posterior_mt, 
                                                              lss_adapter,
                                                              grad_steps, 
                                                              false,
                                                              num_threads);

        vector<double> g = gradient(lss_set);
        reset(lss_set);

        Vector G(g.begin(), g.end());

        long t = 0;
        std::cout << "=========== SERIAL  =======================\n";
        for(size_t i = 0; i < N; i++)
        {
            kjb_c::init_real_time();
            vector<double> g1 = gradient(lss_set);
            t += kjb_c::get_real_time();  
            reset(lss_set);
        }
        double sgt = t / 1000.0;
        cout << "Average Time for serial gradient: " << sgt/N << endl;

        std::cout << "=========== SERIAL POS MT =================\n";
        t = 0;
        for(size_t i = 0; i < N; i++)
        {
            kjb_c::init_real_time();
            vector<double> g_mt = gradient_pmt(lss_set);
            t += kjb_c::get_real_time();  

            Vector G_mt(g_mt.begin(), g_mt.end());
            double diff = vector_distance(G, G_mt);

            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
            }
            TEST_TRUE(diff < epsilon);
            reset(lss_set);
        }
        double smgt = t / 1000.0;
        cout << "Average Time for serial gradient (paralelll posterior): " << smgt/N << endl;

        std::cout << "=========== MT SERIAL gradient =================\n";
        t = 0;
        for(size_t i = 0; i < N; i++)
        {
            kjb_c::init_real_time();
            vector<double> g_mt = gradient_mt(lss_set);
            t += kjb_c::get_real_time();  

            Vector G_mt(g_mt.begin(), g_mt.end());
            double diff = vector_distance(G, G_mt);

            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
                if(diff > epsilon)
                {
                    std::cout << G << "; " << G_mt << std::endl; 
                }
            }
            TEST_TRUE(diff < epsilon);
            reset(lss_set);
        }
        double msgt = t / 1000.0;

        cout << "Average Time for MT serial gradient: " << msgt/N << endl;
        std::cout << "========== MT gradient MT posterior =========\n";
        t = 0;
        for(size_t i = 0; i < N; i++)
        {
            kjb_c::init_real_time();
            vector<double> g_mm = gradient_mmt(lss_set);
            t += kjb_c::get_real_time();  

            Vector G_mm(g_mm.begin(), g_mm.end());
            double diff = vector_distance(G, G_mm);

            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
            }
            TEST_TRUE(diff < epsilon);
            reset(lss_set);
        }
        double mmgt = t / 1000.0;

        cout << "Average Time for MT gradient (paralelll posterior): " << mmgt/N << endl;

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
        throw kjb::Exception(err.what());
    }    
    catch(const exception& ex)
    {
        throw ex;
    }    
}
