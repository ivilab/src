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

/* $Id: generate_synthetic_data.cpp 25797 2020-09-19 21:14:43Z kobus $ */

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

using namespace ivi;
using namespace ivi::ties;
using namespace std;

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

std::string period_coef_mean_str;
std::string period_coef_sigma_str;
std::string damping_coef_mean_str;
std::string damping_coef_sigma_str;
std::vector<double> period_coef_mean;
std::vector<double> period_coef_sigma;
std::vector<double> damping_coef_mean;
std::vector<double> damping_coef_sigma;

double default_mod_coef_prior_mean = 0.0001;
double default_mod_coef_prior_sigma = 0.01;

vector<string> outcomes;

/** @brief  Parse in the coef prior info from the strings. */
std::vector<double> parse_coef_param
(
    const std::string& param_str, 
    size_t num_oscillators,
    bool shared_moderator = false
);

/** @brief  Returns all the means of the coefs. */
std::vector<Vector> get_coef_means
(
    size_t num_oscillators,
    size_t num_outcome = 0,
    double outcome_mean = 0.0,
    bool use_modal = false
);

/** @brief  Returns all the sigmas of the coefs. */
std::vector<Matrix> get_coef_covariances
(
    size_t num_oscillators,
    size_t num_outcome = 0,
    double outcome_sigma = 1.0,
    bool use_modal = false
);

void generate_moderators
(
    const std::string& ld_list_fp, 
    const std::vector<std::string>& moderators,
    size_t num_oscillators,
    Data& data
);

void process_options(int argc, const char** argv);

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */
/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

std::vector<double> parse_coef_param
(
    const std::string& param_str, 
    size_t num_oscillators,
    bool shared_moderator
) 
{
    double temp;
    std::istringstream param_istr(param_str);
    std::vector<double> coef_params;
    size_t d = shared_moderator ? 1 : num_oscillators;
    size_t real_d = d * lss_set_opt.moderators.size() + 1;

    while(param_istr >> temp && real_d > coef_params.size())
    {
        coef_params.push_back(temp);
    }

    IFT(coef_params.size() == real_d, Illegal_argument, 
            "Parameters of the coef has the wrong dimension");
    return coef_params;
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

std::vector<Vector> get_coef_means
(
    size_t num_oscillators,
    size_t num_outcome,
    double outcome_mean,
    bool use_modal
) 
{
    size_t num_params = param_length(num_oscillators, use_modal) + num_outcome;
    std::vector<Vector> coef_means(num_params);

    // parse in the vals
    period_coef_mean = parse_coef_param(period_coef_mean_str, 
                                        num_oscillators);
    std::copy(period_coef_mean.begin(), period_coef_mean.end(), std::ostream_iterator<double>(std::cout, " " ));
    std::vector<double> frequency(period_coef_mean);
    for(size_t i = 0; i < frequency.size(); i++)
    {
        frequency[i] = (2 * M_PI/period_coef_mean[i]);
    }
    std::cout << " freq: ";
    std::copy(frequency.begin(), frequency.end(), std::ostream_iterator<double>(std::cout, " " ));
    std::cout << "\n";
    damping_coef_mean = parse_coef_param(damping_coef_mean_str, 
                                         num_oscillators);

    size_t start_index = 0;
    for(size_t i = 0; i < num_oscillators + 2; i++)
    {
        coef_means[start_index++] = Vector(frequency.begin(), 
                                           frequency.end());
    }
    for(size_t i = 0; i < num_oscillators + 2; i++)
    {
        coef_means[start_index++] = Vector(damping_coef_mean.begin(),
                                           damping_coef_mean.end());
    }
    if(num_outcome > 0)
    {
        for(size_t j = 0; j < num_outcome; j++)
        {
            coef_means[start_index++] = Vector(1, outcome_mean);
        }
    }

    return coef_means;
}

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

std::vector<Matrix> get_coef_covariances
(
    size_t num_oscillators,
    size_t num_outcome,
    double outcome_sigma,
    bool use_modal
) 
{
    size_t num_params = param_length(num_oscillators, use_modal) + num_outcome;
    std::vector<Matrix> coef_covariances(num_params);

    // parse in the vals
    period_coef_sigma = parse_coef_param(period_coef_sigma_str, num_oscillators);
    std::cout << "period coef sigma: " << period_coef_sigma << std::endl;
    damping_coef_sigma = parse_coef_param(damping_coef_sigma_str, num_oscillators);
    std::cout << "damping coef sigma: " << damping_coef_sigma << std::endl;
    size_t start_index = 0;
    //size_t param_per_ddx = num_oscillators + 1; // == 2 + n - 1;

    /*for(size_t i = 0; i < num_oscillators; i++)
    {
        coef_covariances[start_index++] = create_diagonal_matrix(
                Vector(1, M_PI));
    }*/
    for(size_t i = 0; i < 2 * num_oscillators; i++)
    {
        coef_covariances[start_index++] = create_diagonal_matrix(
                Vector(period_coef_sigma.begin(), period_coef_sigma.end()));
    }
    for(size_t i = 0; i < 2 * num_oscillators; i++)
    {
        coef_covariances[start_index++] = create_diagonal_matrix(
                Vector(damping_coef_sigma.begin(), damping_coef_sigma.end()));
    }

    if(num_outcome > 0)
    {
        for(size_t j = 0; j < num_outcome; j++)
        {
            coef_covariances[start_index++] = create_diagonal_matrix
                (Vector(1, outcome_sigma));
        }
    }


    // converting sigmas to covariances 
    for(size_t i = 0; i < coef_covariances.size(); i++)
    {
        for(size_t j = 0; j < coef_covariances[i].get_num_rows(); j++)
        {
            double sigma = coef_covariances[i](j, j);
            coef_covariances[i](j, j) = sigma * sigma;
        }
    }


    return coef_covariances;
}


int main(int argc, const char** argv)
{
#ifdef TEST
    //ivi_c::ivi_init();
    //ivi_c::ivi_l_set("heap-checking", "off");
    //ivi_c::ivi_l_set("initialization-checking", "off");
#endif

    try
    {
        ivi_c::ivi_seed_rand_2_with_tod();
        process_options(argc, argv);

        // the output dirs
        std::string lss_dir = out_dir + "/lss";
        std::string data_dir = out_dir + "/data/";

        // create priors 
        size_t num_oscs = lss_opt.num_oscillators;
        size_t num_params = lss_opt.ignore_clo ? 0 :
            param_length(lss_opt.num_oscillators, lss_opt.use_modal); 
        if(lss_opt.polynomial_degree >= 0)
        {
            lss_opt.num_params += (lss_opt.num_oscillators * 
                                  (lss_opt.polynomial_degree + 1));
        }
        lss_opt.num_params = num_params;

        Group_map group_map;

        Shared_scale_prior scale_prior(
                        vector<double>(
                             num_params,
                             prior_opt.gp_scale_shape), 
                        vector<double>(
                             num_params,
                             prior_opt.gp_scale_scale));

        Shared_noise_prior noise_prior(vector<double>(ll_opt.obs_names.size(),
                                        prior_opt.obs_noise_shape),
                                        vector<double>(ll_opt.obs_names.size(),
                                        prior_opt.obs_noise_scale));

        // time distribution
        Poisson_distribution time_dist(50);

        // init state distribution
        Init_prior init_prior(prior_opt.init_state_mean,
                              prior_opt.init_state_sigma,
                              lss_opt.num_oscillators);
        
        const std::vector<Gaussian_distribution>& init_dists = 
                                        init_prior.distributions();

        // Obs coef distribution
        Normal_distribution obs_coef_P(100, 10.0);

        vector<size_t> ids(num_couples);
        Double_vv all_times(num_couples);
        State_vec init_states((int)num_couples, State_type(2*(int)num_oscs, 0.0));
        vector<Data> all_data(num_couples);

        std::cout << "num_groups: " << cluster_opt.num_groups << std::endl;
        // group prior
        Cluster_prior group_prior(cluster_opt.group_lambda, cluster_opt.num_groups);
        group_prior.init_num_data_per_cluster(std::vector<size_t>(num_couples, 0));


        // if the list is provided, get the same IDs
        if(id_list_fp != "")
        {
            ids = parse_list(id_list_fp);
            size_t num_lss = ids.size();
            if(num_lss != num_couples)
            {
                all_times.resize(num_lss);
                init_states.resize(num_lss);
                all_data.resize(num_lss);
                num_couples = num_lss;
            }

            // read in data
            boost::format data_fmt(data_dp + "/%04d.txt");
            for(size_t i = 0; i < num_lss; i++)
            {
                string fname = (data_fmt % ids[i]).str();
                Data data = parse_data(fname);

                // Use the real couples' times
                all_times[i] = data.times;

                // Use the real couples' initial state 
                init_states[i] = estimate_init_states(data,
                                                    1.0,
                                                    ll_opt.obs_names[0]);
                // create data
                all_data[i] = data;
            }
        }
        else
        {
            for(size_t i = 0; i < num_couples; i++)
            {
                ids[i] = i + 1;
                
                // sample the time length
                //size_t length = sample(time_dist);
                size_t length = 50;
                
                // times
                Double_v times(length);
                generate(times.begin(), times.end(), 
                              Increment<double>(0.0));
                all_times[i] = times;

                // sample init state
                for(size_t j = 0; j < init_dists.size(); j++)
                {
                    double init_state = sample(init_dists[j]);
                    IVI(ASSERT(j < init_states[i].size()));
                    init_states[i][j] = init_state; 
                }

                // data 
                Data data;
                data.dyid = ids[i];
                // moderators 
                generate_moderators(id_list_fp, 
                                    lss_set_opt.moderators, 
                                    lss_opt.num_oscillators, 
                                    data);

                data.times = times;
                all_data[i] = data;
            }
        }

        Vector obs_sigmas = noise_prior.generate();
        std::cout << "obs sigma: " << obs_sigmas << std::endl;
        std::string group_fp(data_dir + "/group_info.txt");
        std::cout << "use_group: " << use_group << std::endl;
        use_group = cluster_opt.num_groups > 1 ? true : false;
        std::cout << "use_group: " << use_group << std::endl;
        std::string group_str = "";
        if(use_group)
        {
            group_str = "Group";
            for(size_t g = 0; g < cluster_opt.num_groups; g++)
            {
                std::string group_str = group_str + "-" 
                                + boost::lexical_cast<std::string>(g);
                group_map.insert(Group_map_entry(g, group_str));
            }
            ivi_c::ivi_mkdir(data_dir.c_str());
            write_group(group_map, data_dir);
        }
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
                        false,
                        group_str,
                        group_fp,
                        lss_opt.use_modal);
        // sample the cluster assignemnt
        if(use_group)
        {
            group_prior.sample_cluster_assignments(lss_set, true);
            lss_set.init_design_matrix();
            Vector weights = group_prior.sample_cluster_weights_from_prior();
            assert(weights.size() == lss_set.num_groups());
            for(size_t g = 0; g < lss_set.num_groups(); g++)
            {
                lss_set.group_weight(g) = weights[g];
            }
        }

        //std::vector<Vector> means = lss_set.pred_coefs();
        std::vector<Vector> means = get_coef_means(num_oscs, 
                                        lss_set.num_outcomes(),
                                        0.0,
                                        lss_opt.use_modal);
        std::cout << "means: ";
        std::copy(means.begin(), means.end(), std::ostream_iterator<Vector>
                (std::cout, "\n"));
        std::cout << endl;
        std::vector<Matrix> covariances = get_coef_covariances(num_oscs,
                                            lss_set.num_outcomes(),
                                            prior_opt.outcome_sigma,
                                            lss_opt.use_modal);

        std::cout << "covariances: ";
        BOOST_FOREACH(const Matrix& co, covariances)
        {
            std::cout << co << std::endl;
        }
        Vector shapes = build_vector(lss_set.clo_param_size(), 
                                 prior_opt.clo_sigma_shape,
                                 lss_set.num_polynomial_params(), 
                                 prior_opt.poly_sigma_shape, 
                                 lss_set.num_outcomes(),
                                 prior_opt.outcome_sigma_shape);
        std::cout << "shapes: " << shapes << std::endl;
        Vector scales = build_vector(lss_set.clo_param_size(), 
                                 prior_opt.clo_sigma_scale,
                                 lss_set.num_polynomial_params(),
                                 prior_opt.poly_sigma_scale,
                                 lss_set.num_outcomes(),
                                 prior_opt.outcome_sigma_scale);
        std::cout << "scales: " << scales << std::endl;
        Independent_blr_prior mean_var_prior(shapes, scales, means, covariances, 
                                            lss_set.get_X_ts(),
                                            lss_set.get_X_t_Xs(),
                                            prior_opt.variance_prior_type,
                                            use_group);

        const double MAX = 20.0;
        const double MIN = -20.0;
        //const double MAX = DBL_MAX;
        //const double MIN = -DBL_MAX;
        bool resample = true;
        // check the frequencey values 
        size_t counts = 0;
        const size_t max_counts = 1000;
        while(counts < max_counts && resample) 
        {
            std::cout << "counts: " << counts << std::endl;
            ivi_c::ivi_seed_rand_2_with_tod();
            long ltime = time(NULL);
            int stime = (unsigned) ltime /2;  
            srand(stime);
            ergo::global_rng<boost::mt19937>().seed(stime);
            resample = false;

            // resample the moderator
            if(!lss_set_opt.moderators.empty())
            {
                std::cout << "sample moderators\n";
                const std::vector<std::string>& mods = 
                                    lss_set_opt.moderators;
                for(size_t d = 0; d < all_data.size(); d++)
                {
                    all_data[d].moderators.clear();
                    generate_moderators(id_list_fp, 
                                        lss_set_opt.moderators, 
                                        lss_opt.num_oscillators, 
                                        all_data[d]);
                    lss_set.lss_vec()[d].init_predictors(
                            all_data[d].moderators,
                            lss_set.get_mod_names());
                }
            }
            // sample the init state
            std::cout << "sample init\n";
            BOOST_FOREACH(Linear_state_space& lss, lss_set.lss_vec())
            {
                lss.changed_index() = 0;
                for(size_t i = 0; i < init_dists.size(); i++)
                {
                    lss.init_state()[i] = sample(init_dists[i]);
                }
            }

            std::vector<std::vector<std::pair<Vector, double> > > mean_var = 
                                mean_var_prior.generate_samples_from_prior(lss_set);
            for(size_t g = 0; g < mean_var.size(); g++)
            {
                std::cout << "pred_coefs: ";
                for(size_t i = 0; i < mean_var[g].size(); i++)
                {
                    lss_set.pred_coefs(g)[i] = mean_var[g][i].first; 
                    lss_set.variances(g)[i] = mean_var[g][i].second;
                    std::cout << mean_var[g][i].first << " ";
                }
               
                std::cout << std::endl;
                std::cout << "variances: ";
                for(size_t i = 0; i < mean_var[g].size(); i++)
                {
                    std::cout << mean_var[g][i].second << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "updated means\n";
            lss_set.update_means();
            // sample over the sigmas if mean is not fixed
            if(!prior_opt.fixed_clo)
            {
                std::cout << "update the variances: \n";
                lss_set.update_variances();
            }
            if(!sample_lss(lss_set, MAX, MIN))
            {
                std::cout << " need to resample\n";
                resample = true;
                counts++;
                continue;
            }
            std::cout << "finish sampling lss \n";

            // sample the obs_coefs
            if(ll_opt.obs_names.size() > 1) 
            {
                std::cout << "sample obs-coefs\n";
                const std::vector<vector<Vector> >& obs_coefs = 
                                            lss_set.obs_coefs();
                // start from the second observable
                for(size_t i = 1; i < obs_coefs.size(); i++)
                {
                    for(size_t j = 0; j < obs_coefs[i].size(); j++)
                    {
                        lss_set.set_obs_coef(i, j, Vector(1, 
                                            sample(obs_coef_P)));
                    }
                }
            }

            // ADD NOISES TO THE OBSERVATIONS 
            // prepare the data
            std::cout << "Generate noise \n";
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
            Vector obs_noise_sigmas((int)ll_opt.obs_names.size(), 0.1);
            for(size_t i = 0; i < ll_opt.obs_names.size(); i++) 
            {
                const std::string& obs = ll_opt.obs_names[i];
                std::pair<Vector, Vector> stats = standardize(all_data, obs, false, false);
                //double val = 0.0;
                double data_std = 0.0;
                for(size_t j = 0; j < stats.second.size(); j++)
                {
                    //val += std::fabs(stats.first[j]);
                    data_std += stats.second[j];
                }
                //val /= stats.first.size();
                obs_noise_sigmas[i] = data_std/stats.second.size() * noise_ratio;
            }
            std::cout << "noise-sigma: " << obs_noise_sigmas << std::endl;

            // finally, add noise to the data
            for(size_t i = 0; i < num_couples; i++)
            {
                std::cout << i << " set obs_noise_sigmas: " << obs_noise_sigmas << std::endl;
                lss_set.lss_vec()[i].noise_sigmas() = obs_noise_sigmas;
                if(!sample_noise_data(lss_set.lss_vec()[i], all_data[i]))
                {
                    resample = true;
                    counts++;
                    break;
                }
                else
                {
                    resample = false;
                }
            }

            // sample outcome variables 
            for(size_t i = 0; i < all_data.size(); i++)
            {
                for(size_t j = 0; j < outcomes.size(); j++)
                {
                    string name = outcomes[j];
                    all_data[i].outcomes[name] = 
                        Double_v((int)lss_opt.num_oscillators, 0.0);
                    for(size_t k = 0; k < lss_opt.num_oscillators; k++)
                    {
                        all_data[i].outcomes[name][k] = 
                            lss_set.lss_vec()[i].sample_outcome(j, k);
                    }
                }
            }
        }

        if(counts == max_counts && resample)
        {
            std::cout << "Unsuccesful generate data\n";
            return EXIT_SUCCESS;
        }

        cout << "writing out model: " << endl;
        // write out the lss_set
        lss_set.write(lss_dir);
        // write out the data 
        ETX(ivi_c::ivi_mkdir(data_dir.c_str()));
        cout << "writing out data: " << endl;
        const vector<Linear_state_space>& lss_vec = lss_set.lss_vec();
        if(use_group)
        {
            get_data_group(all_data, data_dir, group_map);
        }
        else
        {
            write_data(all_data, data_dir);
        }
        std::cout << "finish writing data\n";

        if(visualize)
        {
            for(size_t i = 0; i < num_couples; i++)
            {
                string vis_dp = (boost::format(out_dir + "/plots/%04d/") 
                                    % (all_data[i].dyid)).str();
                if(use_group)
                {
                    size_t gindex = lss_vec[i].group_index();
                    vis_dp = (boost::format(out_dir + "/plots/%d/%04d/")
                                    % gindex % all_data[i].dyid).str();
                }
                ETX(ivi_c::ivi_mkdir(vis_dp.c_str()));
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
                "A file contains all the training couple ids")
            ("noise-ratio,R", bpo::value<double>(&noise_ratio)->default_value(0.5),
                "A directory that contains a lss_set")
            ("period-coef-mean", 
                bpo::value<std::string>(&period_coef_mean_str), 
                "mean values of period-dependent coefficients (values seperated by space")
            ("period-coef-sigma", 
                bpo::value<std::string>(&period_coef_sigma_str), 
                "sigma values of period-dependent coefficients (values sperated by space")
            ("damping-coef-mean", 
                bpo::value<std::string>(&damping_coef_mean_str), 
                "mean values of damping-dependent coefficients (values seperated by space")
            ("damping-coef-sigma", 
                bpo::value<std::string>(&damping_coef_sigma_str), 
                "sigma values of damping-dependent coefficients (values sperated by space")
            ("outcome", 
                bpo::value<vector<string> >(&outcomes)->multitoken(), 
                "outcome variables");

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

            //ivi_c::ivi_cleanup();
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
        IVI_THROW_2(Exception, err.what());
    }    
    catch(const exception& ex)
    {
        throw ex;
    }    
}
