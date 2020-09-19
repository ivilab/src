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

/* $Id: lss_hmc.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <l/l_sys_debug.h>
#include <l_cpp/l_exception.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <gp_cpp/gp_mean.h>

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
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/posterior.h"
#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/sample_lss.h"

using namespace ergo;
using namespace ivi;
using namespace ivi::ties;
using namespace std;

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

size_t rand_seed;
string out_dp;
string in_dp;
string init_state_dp;
double grad_step_size;
double hmc_step_size;
size_t num_leapfrog_steps;
size_t hmc_iters;
bool write_samples;
bool write_proposals;
bool read_model;
bool store_steps;

Data_options data_opt;
Lss_options lss_opt;
Lss_set_options lss_set_opt;
Prior_options prior_opt;
Likelihood_options ll_opt;

/** @brief options handler  */
void process_options(int argc, const char** argv);

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
        ivi_c::init_real_time();
        // process options
        process_options(argc, argv);

        // random seed
        seed_sampling_rand(rand_seed);
        ivi_c::ivi_seed_rand_2(rand_seed);
        srand(rand_seed);
        ergo::global_rng<boost::mt19937>().seed(rand_seed);

        // Read in data
        cout << "Read in data ..." << endl;
        Data data = parse_data(data_opt.data_fname);

        // create Linear_state_space 
        Linear_state_space cur_lss;
        if(read_model)
        {
            cur_lss.read(in_dp);
        }
        else
        {
            size_t length = std::ceil(data.times.size() * 
                                    ll_opt.training_percent);
            Double_v times(length);
            std::generate(times.begin(), times.end(), Increment<double>(0.0));

            Double_v params = coupled_oscillator_params(
                                                    lss_opt.num_oscillators,
                                                    lss_opt.init_period,
                                                    lss_opt.init_damping, 
                                                    lss_opt.use_modal, 
                                                    true);

            // Use the dial values as the initial values
            State_type init_state;
            if(init_state_dp != "")
            {
                Linear_state_space init_state_lss;
                init_state_lss.read(init_state_dp);
                std::cout << init_state_dp << std::endl;
                init_state = init_state_lss.init_state();
            }
            else
            {
                Obs_map::const_iterator it = data.observables.begin();
                init_state = estimate_init_states(
                                             data, 
                                             ll_opt.training_percent,
                                             it->first);
            }

            Vector obs_sigmas((int)ll_opt.obs_names.size(), 
                              ll_opt.init_noise_sigma);
            Coupled_oscillator_v clos(times.size() - 1, 
                              Coupled_oscillator(params, lss_opt.use_modal));
            cur_lss = Linear_state_space(times, 
                                         init_state, 
                                         clos,
                                         ll_opt.obs_names,
                                         obs_sigmas);
        }

        size_t num_co = cur_lss.coupled_oscillators().size();

        // Default init-state prior
        Init_prior init_prior(prior_opt.init_state_mean, 
                              prior_opt.init_state_sigma,
                              lss_opt.num_oscillators);

        size_t num_noise = cur_lss.noise_sigmas().size();
        std::vector<double> noise_shape(num_noise, prior_opt.obs_noise_shape);
        std::vector<double> noise_scale(num_noise, prior_opt.obs_noise_scale);
        Shared_noise_prior noise_prior(noise_shape, noise_scale);
        // Default Lss prior
        bool use_lss_prior = false;

        // read in shared priors if available 
        if(prior_opt.prior_fp != "")
        {
            cur_lss.init_predictors(data.moderators, 
                    lss_set_opt.get_all_moderators(
                            lss_opt.ignore_clo,
                            lss_opt.num_oscillators,
                            lss_opt.num_params));

            Group_params group_params;
            size_t clo_param_size = param_length(lss_opt.num_oscillators);
            parse_shared_params(prior_opt.prior_fp,
                                group_params);
            // means
            const std::vector<Vector>& preds = cur_lss.get_predictors();
            size_t num_params = group_params.pred_coefs.size();
            IVI(ASSERT(group_params.pred_coefs.size() == preds.size()));
            for(size_t i = 0; i < num_params; i++)
            {
                double val = dot(preds[i], group_params.pred_coefs[i]);
                if(cur_lss.allow_drift())
                {
                    cur_lss.set_gp_mean(i, gp::Constant(val));
                }
                else
                {
                    cur_lss.set_clo_mean(i, val);
                }
            }
            // sigmas 
            for(size_t i = 0; i < group_params.variances.size(); i++)
            {
                cur_lss.set_clo_variance(i, group_params.variances[i]);
            }

            use_lss_prior = true;
        }

        // likelihood
        Likelihood likelihood(data);

        // posterior
        Posterior posterior(likelihood, 
                            init_prior, 
                            true,
                            use_lss_prior);

        // hmc step sizes 
        bool sample_init_state = init_state_dp == "" ? true : false;

        Lss_adapter adapter(sample_init_state);
        size_t sampled_num_params = adapter.size(&cur_lss);

        size_t num_times = data.times.size();
        double step_size = hmc_step_size * (1.0/std::pow(num_times, 3.0));
        std::cout << " hmc_step_size: " << step_size << std::endl;

        vector<double> hmc_step_sizes(sampled_num_params, step_size); 
        vector<double> ubs;
        vector<double> lbs;
        ubs.resize(sampled_num_params);
        lbs.resize(sampled_num_params);
        fill(lbs.begin(), lbs.end(), -numeric_limits<double>::max());
        fill(ubs.begin(), ubs.end(), numeric_limits<double>::max());
        ubs[0] = 0.0;
        ubs[1] = 0.0;
        ubs[2] = 0.0;

        Linear_state_space best_lss(cur_lss);

        hmc_step<Linear_state_space> step(
                            adapter,
                            posterior, 
                            Lss_gradient<Lss_adapter>(posterior, 
                                                      adapter, 
                                                      grad_step_size),
                            hmc_step_sizes, 
                            num_leapfrog_steps,
                            0.0);

        //step.store_leapfrog_steps(store_steps);

        // record the samples and lls
        std::cout << "creating output directory\n";
        boost::format out_fmt(out_dp + "/%04d");
        out_dp = (out_fmt % data.dyid).str();
        ETX(ivi_c::ivi_mkdir(out_dp.c_str()));
        string log_fname = out_dp + "/sample_log.txt";
        string bst_fname = out_dp + "/ll.txt";

        ofstream log_fs(log_fname.c_str(), std::ofstream::app);
        ofstream bst_fs(bst_fname.c_str(), std::ofstream::app);
        if(log_fs.fail())
        {
            IVI_THROW_3(IO_error, "Can't open file %s", (log_fname.c_str()));
        }
        if(log_fs.fail())
        {
            IVI_THROW_3(IO_error, "Can't open file %s", (bst_fname.c_str()));
        }
        // recorders
        step.add_recorder(make_best_sample_recorder(&best_lss).replace());
        step.add_recorder(make_hmc_detail_recorder(
             ostream_iterator<ergo::step_detail>(log_fs, "\n")));
        ergo::best_target_recorder<ostream_iterator<double> > 
            best_target_recorder(ostream_iterator<double>(bst_fs, "\n"));

        step.add_recorder(best_target_recorder);

        string proposals_dir = out_dp + "/proposals";
        string samples_dir = out_dp + "/samples";
        Linear_state_space_recorder record_sample(samples_dir);
        Linear_state_space_recorder record_proposals(proposals_dir);

        if(write_samples)
        {
            ETX(ivi_c::ivi_mkdir(samples_dir.c_str()));
        }
        if(write_proposals)
        {
            ETX(ivi_c::ivi_mkdir(proposals_dir.c_str()));
            step.store_proposed(true);
        }

        // run the sampler 
        double cur_target = posterior(cur_lss);
        double prev_best_target = cur_target;
        double best_target = cur_target;
        size_t num_accepted = 0;
        size_t num_iter = hmc_iters * cur_lss.get_times().size();
        for(size_t i = 0; i < num_iter; i++)
        {
            step(cur_lss, cur_target);
            if(cur_target > best_target)
            {
                best_target = cur_target;
            }
            if(write_samples)
            {
                record_sample(cur_lss);
            }
            if(write_proposals)
            {
                record_proposals(*step.proposed_model());
            }
                   
            static size_t iter_to_check = 50;
            static bool not_record = false;
            adapt_hmc_step_size<Linear_state_space>(
                                               step, 
                                               log_fs, 
                                               i, 
                                               num_accepted, 
                                               iter_to_check,
                                               not_record);

            // write down the current best result
            best_lss.write(out_dp);
            if(!not_record)
            {
                log_fs.flush();
                bst_fs.flush();
            }
            if(i % 1000 == 0)
            {
                best_lss.write(out_dp);
            }
            // write down the current best result
            if((i+1) % 3000 == 0)
            {
                if(fabs(prev_best_target - best_target) < 0.01)
                {
                    break;
                }
                else
                {
                    prev_best_target = best_target;
                }
            }
        }

        // plot
        Linear_state_space lss_all = plot_data_and_model(data, 
                                                         best_lss, 
                                                         out_dp);
        lss_all.write(string(out_dp + "/all_states"));
        best_lss.write(out_dp);
        long st = ivi_c::get_real_time();

        // compute the error 
        std::vector<Vector> obs_errors;
        Vector error = compute_ave_error(data, lss_all, obs_errors, 
                                         ll_opt.training_percent);
        string err_fp(out_dp + "/rms_error.txt");
        ofstream err_of(err_fp.c_str());
        IFTD(err_of.is_open(), IO_error, "can't open file %s", (err_fp.c_str()));
        err_of << data.dyid << " " << error << std::endl;
        std::cout << "Exectution time fitting: " << st / 1000.0 << "s" << std::endl;

        // Compute the predictive distribution
        ivi_c::init_real_time();
        string sample_dp(out_dp + "/pred_samples");
        ETX(ivi_c::ivi_mkdir(sample_dp.c_str()));
        string sample_target_fp(sample_dp + "/ll.txt");
        string sample_log_fp(sample_dp + "/sample_log.txt");
        size_t num_samples = 500;
        bool not_record = false;

        typedef boost::optional<const string&> Opt_str;
        std::vector<Linear_state_space> samples = generate_lss_samples(
                                                        best_lss, 
                                                        posterior, 
                                                        num_samples,
                                                        Opt_str(sample_target_fp),
                                                        Opt_str(sample_log_fp),
                                                        not_record);
            
        boost::format sample_fmt(sample_dp + "/sample_%04d/");
        string pred_fp(sample_dp + "/pred_sample_error.txt");
        ofstream ofs(pred_fp.c_str());
        Vector errors(4, 0.0); 
        // compute the distribution 
        for(size_t i = 0; i < num_samples; i++)
        {
            //prs[i] = std::exp(posterior(samples[i]));
            string sample_sub_dp = (sample_fmt % (i+1)).str();
            samples[i].write(sample_sub_dp);
            std::vector<Vector> obs_errors;
            errors += compute_ave_error(data, samples[i], obs_errors); 
        }
        // predictive distribution 
        ofs << data.dyid << " "; 
        errors /= num_samples;
        BOOST_FOREACH(double val, errors)
        {
            ofs << val << " ";
        }
        ofs << endl;

        st = ivi_c::get_real_time();
        std::cout << "Execution time of predicting samples: " << st / 1000.0 << "s" << std::endl;
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
            ("out-dp,O",
                po::value<string>(&out_dp)->required(),
                "Output directory");

        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("config-file,C",
                po::value<string>(),
                "path of file containing program options in opt=val format")
            ("random-seed", 
                po::value<size_t>(&rand_seed)->default_value(1000), 
                "random seed")
            ("grad-step-size", 
                po::value<double>(&grad_step_size)->default_value(0.0001),
                "Gradient step size of CLO parameters")
            ("hmc-step-size", 
                po::value<double>(&hmc_step_size)->default_value(0.0001),
                "Step size of HMC")
            ("num-leapfrog-steps",
                po::value<size_t>(&num_leapfrog_steps)->default_value(10),
                "Number of leapfrog steps during one HMC iteration")
            ("num-iterations",
                po::value<size_t>(&hmc_iters)->default_value(10),
                "Number of leapfrog steps during one HMC iteration")
            ("write-samples", 
                po::bool_switch(&write_samples)->default_value(false),
                "If set, write samples of each HMC iteration")
            ("write-proposals", 
                po::bool_switch(&write_proposals)->default_value(false),
                "If set, write proposals of each HMC iteration")
            ("store-leapfrogs", 
                po::bool_switch(&store_steps)->default_value(false),
                "If set, store leapfrogs and visualize them")
            ("input-dir", 
                po::value<string>(&in_dp), "Input directory")
            ("init-state-dir", 
                po::value<string>(&init_state_dp), 
                "Directory for the Linear_state_space of initial state");

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
        pstnl.add("out-dp", 1);

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

        if(vm.count("input-dir"))
        {
            read_model = true;
        }
        else
        {
            read_model = false;
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

