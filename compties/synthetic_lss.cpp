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

/* $Id: synthetic_lss.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <m_cpp/m_vector.h>
#include <p/p_plot.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>

#include <vector>
#include <utility>
#include <string> 
#include <ostream>

#include <boost/lexical_cast.hpp>
#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>


#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/coupled_oscillator.h"
#include "dbn_cpp/typedefs.h"
#include "dbn_cpp/prior.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

size_t num_lss;
string out_dir;
bool visualize;
size_t random_seed;
Data_options data_opt;
Lss_options lss_opt;
Prior_options prior_opt;
Likelihood_options ll_opt;

void process_options(int argc, const char** argv);

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
        seed_sampling_rand(random_seed);

        // init state prior
        Init_prior init_prior(prior_opt.init_state_mean, 
                              prior_opt.init_state_sigma,
                              lss_opt.num_oscillators);

        // time distribution 
        Poisson_distribution time_dist(50);

        string data_dp(out_dir + "/data/");
        string lss_dp(out_dir + "/lss/");

        ETX(ivi_c::ivi_mkdir(data_dp.c_str()));
        ETX(ivi_c::ivi_mkdir(lss_dp.c_str()));


        // BMI distributions 
        Normal_distribution woman_bmi_dist(23.5, 5.0);
        Normal_distribution man_bmi_dist(23.6, 6.0);

        boost::format data_fmt(data_dp + "/%04d.txt");
        boost::format lss_fmt(lss_dp + "/%04d");
        for(size_t l = 0; l < num_lss; l++)
        {
            std::cout << " sample couple: " << l+1 << std::endl;
            // sample time 
            size_t length = sample(time_dist);
            Double_v times(length);
            std::generate(times.begin(), times.end(), Increment<double>(0.0));

            // init state
            State_type init_state = init_prior.sample();

            // CLO params
            Double_v params = coupled_oscillator_params(
                                                    lss_opt.num_oscillators,
                                                    lss_opt.init_period,
                                                    lss_opt.init_damping,
                                                    lss_opt.use_modal);

            size_t num_params = params.size();
            std::vector<gp::Constant> gp_means(num_params, gp::Constant(0));

            double woman_bmi = sample(woman_bmi_dist);
            double man_bmi = sample(man_bmi_dist);
            double bmi_ave = (woman_bmi + man_bmi)/2.0;
            double bmi_diff = woman_bmi - man_bmi;
            Coupled_oscillator_v clos(times.size() - 1, 
                                      Coupled_oscillator(params, lss_opt.use_modal));
            Linear_state_space lss(times, 
                                   init_state, 
                                   clos,
                                   ll_opt.obs_names);
            if(lss_opt.drift)
            {
                // gp stuff 
                std::copy(params.begin(), params.end(), gp_means.begin());

                Double_v gp_scales((int)num_params, prior_opt.gp_scale);
                Double_v gp_sigvars((int)num_params, prior_opt.clo_sigma * prior_opt.clo_sigma);

                lss.init_gp(gp_scales, gp_sigvars, gp_means);

                // sample from the gp prior
                lss.sample_clo_from_gp_prior();
            }

            // Add noise to the states
            Data data;
            data.dyid = (l+1);
            data.times = times; 
            const State_vec_vec& states = lss.get_states();
            State_vec_vec sampled_states;
            std::copy(states.begin(), states.end(), back_inserter(sampled_states));
            double obs_noise = ll_opt.init_noise_sigma;
            data.observables["dial"] = Vector_v(lss_opt.num_oscillators);

            double max = -DBL_MAX;
            double min = DBL_MAX;
            for(size_t m = 0; m < states.size(); m++)
            {
                // Add in noise 
                for(size_t n = 0; n < states[m].size(); n++)
                {
                    Gaussian_distribution P(0.0, obs_noise);
                    sampled_states[m][0][n] += sample(P); 
                    // check the extream values 
                    if(sampled_states[m][0][n] > max)
                    {
                        max = sampled_states[m][0][n];
                    }
                    if(sampled_states[m][0][n] <= min)
                    {
                        min = sampled_states[m][0][n];
                    }

                    data.observables.at("dial")[m].push_back(sampled_states[m][0][n]);
                }
            }
            if(max > 3.0 || min < -3.0)
            {
                l--;
                continue;
            }

            // write data
            string data_fp = (data_fmt % data.dyid).str();
            write_data(data, data_fp);

            // write lss
            string lss_fp = (lss_fmt % (l+1)).str();
            lss.write(lss_fp);

            // visualize
            if(visualize)
            {
                string vis_dp = (boost::format(out_dir + "/plots/%04d/") 
                                    % (data.dyid)).str();
                ETX(ivi_c::ivi_mkdir(vis_dp.c_str()));
                plot_data_and_model(data, lss, vis_dp, false);
            }
        }
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
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }

    return EXIT_SUCCESS;
}

namespace bpo = boost::program_options;
void process_options(int argc, const char** argv)
{
    bpo::variables_map vm;
    try
    {
        // Hidden options
        bpo::options_description hidden_options("Hidden options");
        hidden_options.add_options()
            ("output-dir,O",
                bpo::value<string>(&out_dir)->required(),
                "Output directory");

        // General options
        bpo::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("config-file,C",
                bpo::value<string>(),
                "path of file containing program options in opt=val format")
            ("random-seed", 
                bpo::value<size_t>(&random_seed)->default_value(1000), 
                "random seed")
            ("num-lss", 
                bpo::value<size_t>(&num_lss)->default_value(10), 
                "Number of Linear_state_space")
            ("visualize", 
                bpo::bool_switch(&visualize)->default_value(false),
                "if set visualize the synthetic data");


        bpo::options_description data_options = make_options(data_opt);
        bpo::options_description lss_options = make_options(lss_opt);
        bpo::options_description prior_options = make_options(prior_opt);
        bpo::options_description ll_options = make_options(ll_opt);

        // Combine options
        bpo::options_description visible_options;
        visible_options.add(generic_options)
                       .add(data_options)
                       .add(lss_options)
                       .add(prior_options)
                       .add(ll_options);

        bpo::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        bpo::positional_options_description pstnl;
        pstnl.add("output-dir", 1);

        // process options
        bpo::store(bpo::command_line_parser(argc, argv)
                .options(cmdline_options).positional(pstnl).run(), vm);

        if(vm.count("help"))
        {
            std::cout << "Usage: ties_sampler OPTIONS MOVIE-RUN-DIR\n"
                      << visible_options << "\n"
                      << "For questions or complaints please contact "
                      << "jguan1@email.arizona.edu.\n" << std::endl;

            //ivi_c::ivi_cleanup();
            exit(EXIT_SUCCESS);
        }

        // open config file if it is given
        if(vm.count("config-file"))
        {
            string config_file_name = vm["config-file"].as<string>();
            process_config_file(config_file_name, visible_options, vm);
        }

        // notify
        bpo::notify(vm);
    }
    catch(const bpo::error& err)
    {
        IVI_THROW_2(Exception, err.what());
    }    
    catch(const std::exception& ex)
    {
        throw ex;
    }    
}
