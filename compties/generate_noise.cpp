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

/* $Id: generate_synthetic_data.cpp 20895 2016-10-20 06:36:56Z jguan1 $ */

#include <m_cpp/m_vector.h>
#include <p/p_plot.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>

#include <vector>
#include <utility>
#include <string> 
#include <ostream>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/prior.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/sample_lss.h"
#include "dbn_cpp/real_data_parser.h"

#define FREQ_1 0
#define DAMP_1 1
#define COUP_1 2
#define FREQ_2 3
#define DAMP_2 4
#define COUP_2 5

using namespace kjb;
using namespace kjb::ties;
using namespace std;

string in_dir;
string out_dir;
double noise_ratio;
size_t rand_seed;
size_t num_couples;
string id_list_fp;
string data_dp;
Lss_options lss_opt;
Lss_set_options lss_set_opt;
Prior_options prior_opt;
Cluster_options cluster_opt;
Likelihood_options ll_opt;
bool visualize;
bool use_group;

void process_options(int argc, const char** argv);

void generate_moderators
(
    const std::string& ld_list_fp, 
    const std::vector<std::string>& moderators,
    size_t num_oscillators,
    Data& data
);

int main(int argc, const char** argv)
{
#ifdef TEST
    //kjb_c::kjb_init();
    //kjb_c::kjb_l_set("heap-checking", "off");
    //kjb_c::kjb_l_set("initialization-checking", "off");
#endif

    try
    {
        kjb_c::kjb_seed_rand_2_with_tod();
        process_options(argc, argv);

        std::string lss_set_dp(argv[1]);
        std::string out_dp(argv[2]);

        // create priors 
        size_t num_oscs = lss_opt.num_oscillators;
        size_t num_params = param_length(num_oscs);


        // Obs coef distribution
        Normal_distribution obs_coef_P(100, 10.0);

        std::string data_dp(in_dir + "/data/");
        std::string id_list_fp(data_dp + "/ids.txt");
        vector<size_t> ids = parse_list(id_list_fp);
        num_couples = ids.size();
        Double_vv all_times(num_couples);
        State_vec init_states(num_couples, State_type(2*(int)num_oscs, 0.0));
        vector<Data> all_data = parse_data_from_dir(data_dp);

        std::vector<std::string> outcomes;
        Vector obs_sigmas((int)ll_opt.obs_names.size(), 0.0);
        Lss_set lss_set(ids,
                        lss_set_opt.get_all_moderators(
                            lss_opt.ignore_clo,
                            lss_opt.num_oscillators,
                            lss_opt.num_params),
                        init_states,
                        all_data,
                        prior_opt.gp_scale,
                        prior_opt.clo_sigma,
                        prior_opt.poly_sigma,
                        prior_opt.outcome_sigma,
                        ll_opt.obs_names,
                        prior_opt.fixed_clo,
                        1.0,
                        lss_opt.num_oscillators,
                        lss_opt.init_period,
                        lss_opt.init_damping,
                        obs_sigmas,
                        lss_opt.drift,
                        cluster_opt.num_groups,
                        lss_opt.polynomial_degree,
                        outcomes,
                        false);
        std::string lss_in_dir(in_dir + "/lss"); 
        lss_set.read(lss_in_dir, all_data);

        // ADD NOISES TO THE OBSERVATIONS 
        // prepare the data
        for(size_t i = 0; i < num_couples; i++)
        {
            int time_length = all_times[i].size();
            if(id_list_fp == "")
            {
                // Initialize data
                for(size_t j = 0; j < ll_opt.obs_names.size(); j++)
                {
                    const std::string& obs_name = ll_opt.obs_names[j];
                    all_data[i].observables[obs_name] = 
                        Vector_v(num_oscs, Vector(time_length));
                }
            }
            // generate noise free data j
            sample_noise_data(lss_set.lss_vec()[i], all_data[i], true);
        }

        // compute the variance of the data of each observable
        Vector obs_noise_sigmas((int)ll_opt.obs_names.size(), 0.0);
        for(size_t i = 0; i < ll_opt.obs_names.size(); i++) 
        {
            const std::string& obs = ll_opt.obs_names[i];
            std::pair<Vector, Vector> stats = standardize(all_data, obs, false, false);
            assert(stats.first.size() == lss_opt.num_oscillators);
            double val = 0.0;
            for(size_t j = 0; j < stats.first.size(); j++)
            {
                val += std::fabs(stats.first[j]);
            }
            val /= stats.first.size();
            obs_noise_sigmas[i] = val * noise_ratio;
            std::cout << obs << ": " << obs_noise_sigmas[i] << std::endl;
        }

        // finally, add noise to the data
        for(size_t i = 0; i < num_couples; i++)
        {
            lss_set.lss_vec()[i].noise_sigmas() = obs_noise_sigmas;
            std::cout << "couple " << i << std::endl;
            sample_noise_data(lss_set.lss_vec()[i], all_data[i], false);
        }

        cout << "writing out model: " << endl;
        // write out the lss_set
        lss_set.write(out_dir + "/lss/");
        // write out the data 
        string data_dir(out_dir + "/data/");
        ETX(kjb_c::kjb_mkdir(data_dir.c_str()));
        cout << "writing out data: " << endl;
        const vector<Linear_state_space>& lss_vec = lss_set.lss_vec();
        if(use_group)
        {
            get_data_group(all_data, data_dir, lss_set.group_map());
            write_group(lss_set.group_map(), data_dir);
        }
        else
        {
            write_data(all_data, data_dir);
        }
        std::cout << "finish writing data\n";

        for(size_t i = 0; i < num_couples; i++)
        {
            if(visualize)
            {
                string vis_dp = (boost::format(out_dir + "/plots/%04d/") 
                                    % (all_data[i].dyid)).str();
                if(use_group)
                {
                    size_t gindex = lss_vec[i].group_index();
                    vis_dp = (boost::format(out_dir + "/plots/%d/%04d/")
                                    % gindex % all_data[i].dyid).str();
                }
                ETX(kjb_c::kjb_mkdir(vis_dp.c_str()));
                plot_data_and_model(all_data[i], lss_vec[i], vis_dp, false);
                std::cout << "finish visualize " << vis_dp << std::endl;
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

void generate_moderators
(
    const std::string& ld_list_fp, 
    const std::vector<std::string>& moderators,
    size_t num_oscillators,
    Data& data
)
{
    if(ld_list_fp != "")
    {
        Normal_distribution woman_bmi_dist(23.5, 5.0);
        Normal_distribution man_bmi_dist(23.6, 3.0);
        double bmi_a = sample(woman_bmi_dist);
        double bmi_b = sample(man_bmi_dist);
        double bmiave = (bmi_a + bmi_b) /2.0;
        double bmidif = bmi_a - bmi_b;
        for(size_t j = 0; j < moderators.size(); j++)
        {
            string mod_name = moderators[j];
            for(size_t k = 0; k < num_oscillators; k++)
            {
                if(mod_name == "bmiave")
                {
                    data.moderators[mod_name].push_back(bmiave);
                }
                if(mod_name == "bmidif")
                {
                    data.moderators[mod_name].push_back(bmidif);
                }
            }
        }
    }
    else
    {
        Normal_distribution mod_mean_dist(0, 0.01);
        Inverse_gamma_distribution mod_sigma_dist(10.0, 0.01);
        for(size_t m = 0; m < moderators.size(); m++)
        {
            double m_mean = sample(mod_mean_dist);
            double m_sigma = sample(mod_sigma_dist);
            Normal_distribution m_dist(m_mean, m_sigma);
            double mod_val = sample(m_dist);
            string mod_name = lss_set_opt.moderators[m];
            for(size_t k = 0; k < num_oscillators; k++)
            {
                data.moderators[mod_name].push_back(mod_val);
            }
        }
    }
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
                bpo::value<size_t>(&rand_seed)->default_value(1000), 
                "random seed")
            ("num-couples,N", 
                bpo::value<size_t>(&num_couples)->default_value(20), 
                "number of couples")
            ("visualize", 
                bpo::bool_switch(&visualize)->default_value(false),
                "if set visualize the synthetic data")
            ("use-group", 
                bpo::bool_switch(&use_group)->default_value(false),
                "if set use group")
            ("data-dir,D", bpo::value<string>(&data_dp),
                "Data directory, contains all the couple data")
            ("id-list,L", bpo::value<string>(&id_list_fp),
                "A file that contains all the training couple ids")
            ("input-dir,I", bpo::value<string>(&in_dir)->required(),
                "A directory that contains a lss_set")
            ("noise-ratio,R", bpo::value<double>(&noise_ratio)->default_value(0.5),
                "A directory that contains a lss_set");

        bpo::options_description lss_options = make_options(lss_opt);
        bpo::options_description lss_set_options = make_options(lss_set_opt);
        bpo::options_description prior_options = make_options(prior_opt);
        bpo::options_description cluster_options = make_options(cluster_opt);
        bpo::options_description ll_options = make_options(ll_opt);

        // Combine options
        bpo::options_description visible_options;
        visible_options.add(generic_options)
                       .add(lss_options)
                       .add(lss_set_options)
                       .add(prior_options)
                       .add(cluster_options)
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
            cout << "Usage: ties_sampler OPTIONS MOVIE-RUN-DIR\n"
                      << visible_options << "\n"
                      << "For questions or complaints please contact "
                      << "jguan1@email.arizona.edu.\n" << endl;

            //kjb_c::kjb_cleanup();
            exit(EXIT_SUCCESS);
        }
        if(vm.count("num-groups"))
        {
            use_group = true;
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
        KJB_THROW_2(Exception, err.what());
    }    
    catch(const exception& ex)
    {
        throw ex;
    }    
}
