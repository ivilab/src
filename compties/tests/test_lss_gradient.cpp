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

/* $Id: test_lss_gradient.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <l_cpp/l_exception.h>
#include <l_cpp/l_test.h>

#include <ergo/mh.h>
#include <ergo/hmc.h>
#include <ergo/record.h>

#include <iostream>

#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/adapter.h"
#include "dbn_cpp/posterior.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#define VERBOSE 0

using namespace ergo;
using namespace ivi;
using namespace ivi::ties;
using namespace std;

double grad_step_size;

string data_fname;

Lss_options lss_opt;
Prior_options prior_opt;
Likelihood_options likelihood_opt;

int rand_seed;
size_t num_grad_threads;

/** @brief options handler  */
void process_options(int argc, const char** argv);

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

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
        // process options
        process_options(argc, argv);
        cout << " random-seed: " << rand_seed << endl;
        seed_sampling_rand(rand_seed);
        ivi_c::ivi_seed_rand(rand_seed, rand_seed);
        ivi_c::ivi_seed_rand_2(rand_seed);
        ergo::global_rng<boost::mt19937>().seed(rand_seed);

        // Read in data
        cout << "Read in data ..." << endl;
        Data data = parse_data(data_fname);

        // create Linear_state_space 
        size_t length = data.times.size();
        Double_v times = data.times;
        std::map<size_t, std::vector<double> > com_params;
        //com_params.reserve(times.size() - 1);

        Double_v params = coupled_oscillator_params(
                                                lss_opt.num_oscillators,
                                                lss_opt.init_period,
                                                lss_opt.init_damping,
                                                lss_opt.use_modal);

        size_t num_params = params.size();
        int end_index = std::min((int)times.size() - 1, (int)length - 1);
           

        // Use the dial values as the initial values
        State_type init_state = estimate_init_states(data, 
                                                     likelihood_opt.training_percent,
                                                     likelihood_opt.obs_names[0]);

        // gp stuff 
        Double_v gp_scales((int)num_params, prior_opt.gp_scale);
        Double_v gp_sigvars((int)num_params, prior_opt.clo_sigma*prior_opt.clo_sigma);
        std::vector<gp::Constant> gp_means(num_params, gp::Constant(0));
        gp_means[0] = gp::Constant(-0.02);

        Vector obs_sigmas((int)likelihood_opt.obs_names.size(), 
                            likelihood_opt.init_noise_sigma);

        Coupled_oscillator_v clos(times.size() - 1, Coupled_oscillator(params));
        Linear_state_space cur_lss(times, 
                                   init_state, 
                                   clos, 
                                   likelihood_opt.obs_names,
                                   obs_sigmas);

        cur_lss.init_gp(gp_scales, gp_sigvars, gp_means);

        // Prior
        size_t num_co = cur_lss.coupled_oscillators().size();
        Init_prior init_prior; 
        Shared_noise_prior noise_prior;

        // likelihood
        Likelihood likelihood(data);

        // posterior
        Posterior posterior(likelihood, init_prior);

        // hmc step sizes 
        size_t total_num_params = init_state.size();
        BOOST_FOREACH(const Coupled_oscillator& co, cur_lss.coupled_oscillators())
        {
            total_num_params += co.num_params();
        }

        vector<double> grad_steps(total_num_params, grad_step_size);
        vector<double> ubs;
        vector<double> lbs;
        ubs.resize(total_num_params);
        lbs.resize(total_num_params);
        fill(lbs.begin(), lbs.end(), -numeric_limits<double>::max());
        fill(ubs.begin(), ubs.end(), numeric_limits<double>::max());
        ubs[0] = 0.0;
        ubs[1] = 0.0;

        Linear_state_space best_lss(cur_lss);

        Lss_gradient<Lss_adapter> gradient(posterior, Lss_adapter(), grad_step_size);
        Lss_gradient_mt<Lss_adapter> gradient_mt(posterior, 
                                                 Lss_adapter(), 
                                                 grad_step_size, 
                                                 num_grad_threads);
        std::vector<double> g = gradient(cur_lss);
        Vector G1(g.begin(), g.end());
        double t = 0;
        std::cout << "===========  MT gradient =================\n";
        const size_t N = 100;
        const double epsilon = 1e-4;
        for(size_t i = 0; i < N; i++)
        {
            ivi_c::init_real_time();
            cur_lss.changed_index() = 0;
            vector<double> g_mt = gradient_mt(cur_lss);
            t += ivi_c::get_real_time();  

            Vector G2(g_mt.begin(), g_mt.end());
            double diff = vector_distance(G1, G2);

            if(VERBOSE)
            {
                cout << i << " diff: " << diff << endl;
            }
            TEST_TRUE(diff < epsilon);
        }
        double pmgt = t / 1000.0;
        std::cout << "=========== SERIAL  =======================\n";
        t = 0; 
        for(size_t i = 0; i < N; i++)
        {
            ivi_c::init_real_time();
            cur_lss.changed_index() = 0;
            vector<double> g1 = gradient(cur_lss);
            t += ivi_c::get_real_time();  
        }
        double psgt = t / 1000.0;
        cout << "Average Time for serial gradient: " << psgt/N << endl;
        cout << "Average Time for parallel gradient: " << pmgt/N << endl;
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
            ("data-file",
                 po::value<string>(&data_fname)->required(),
                 "The file that contains the data");
        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("config-file,C",
                po::value<string>(),
                "path of file containing program options in opt=val format")
            ("num-grad-threads,T",
                 po::value<size_t>(&num_grad_threads)->default_value(1),
                 "Number of threads of the gradient computation")
            ("grad-step-size", 
                 po::value<double>(&grad_step_size)->default_value(0.0001),
                 "gradient step size")
            ("random-seed", po::value<int>(&rand_seed)->default_value(1000), 
                 "Sampling period");

        po::options_description lss_options = make_options(lss_opt);
        bpo::options_description prior_options = make_options(prior_opt);
        bpo::options_description likelihood_options = make_options(likelihood_opt);

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options)
                       .add(lss_options)
                       .add(prior_options)
                       .add(likelihood_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("data-file", -1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);

        // help called?
        if(vm.count("help"))
        {
            cout << "usage: ./hmc data-file"
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

