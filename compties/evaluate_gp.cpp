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

/* $Id: evaluate_gp.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <l_cpp/l_exception.h>
#include <gp_cpp/gp_predictive.h>
#include <gp_cpp/gp_sample.h>

#include <vector>
#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>
#include <boost/foreach.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/util.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

string data_fname;
string exp_dir;
string vis_dir;
string err_mode;
double gp_scale;
double gp_variance;
double training_percent;
bool visualize;
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
        // process options
        process_options(argc, argv);

        Data data = parse_data(data_fname);

        // read in the linear state 
        Linear_state_space lss;
        lss.read(exp_dir);
        assert(lss.allow_drift());
        size_t new_ed = data.times.size();

        Coupled_oscillator_v& clos = lss.coupled_oscillators();

        size_t num_params = clos.back().num_params();
        size_t old_co_nums = clos.size();
        size_t new_co_nums = new_ed - 1;

        gp::Inputs inputs(old_co_nums);
        gp::Inputs x_star(new_co_nums);
        std::vector<Vector> pts(old_co_nums);
        for(size_t i = 0; i < old_co_nums; i++)
        {
            pts[i] = Vector(clos[i].begin(), clos[i].end());
            inputs[i].set(i + 1.0);
            x_star[i].set(i + 1.0);
        }
        for(size_t i = old_co_nums; i < new_co_nums; i++)
        {
            x_star[i].set(i + 1.0);
        }

        typedef gp::Predictive_nl<gp::Constant, gp::Squared_exponential> Gp_pred; 

        const size_t num_samples = 20;
        std::vector<Vector> errors(num_samples);
        for(size_t s = 0; s < num_samples; s++)
        {
            Double_vv all_params(x_star.size());
            for(size_t i = 0; i < num_params; i++)
            {
                Vector x_outputs(pts.size());
                for(size_t j = 0; j < pts.size(); j++)
                {
                    x_outputs[j] = pts[j][i];
                }

                Gp_pred pred(gp::Constant(0), 
                             gp::Squared_exponential(gp_scale, gp_variance),
                             inputs.begin(),
                             inputs.end(),
                             x_outputs.begin(),
                             x_outputs.end(),
                             x_star.begin(),
                             x_star.end());

                //MV_normal_distribution pred_dist = pred.normal();
                //Vector mu = pred_dist.get_mean();
                Vector mu = sample(pred);
                assert(mu.size() == all_params.size());
                for(size_t j = 0; j < mu.size(); j++)
                {
                    all_params[i].push_back(mu[j]);
                }
            }

            for(size_t i = old_co_nums; i < new_co_nums; i++)
            {
                clos.push_back(Coupled_oscillator(all_params[i]));
            }

            lss.changed_index() = old_co_nums;

            // visualize 
            if(visualize)
            {
                boost::format out_dir_fmt(exp_dir + "/samples/%02d");
                if(vis_dir == "")
                {
                    vis_dir = (out_dir_fmt % (s+1)).str();
                    ETX(ivi_c::ivi_mkdir(vis_dir.c_str()));
                    cerr << " visualize directory is not provided,"
                         << " saving to " << exp_dir << std::endl;
                }
                plot_data_and_model(data, lss, vis_dir, true);
            }

            std::vector<Vector> obs_errors;
            errors[s] = compute_ave_error(data, lss, obs_errors, training_percent);

        }

        Vector ave_errors = std::accumulate(errors.begin(), errors.end(), 
                                             Vector(errors[0].size(), 0.0));
        ave_errors /= num_samples;
        cout << data.dyid << " "; 
        for(size_t i = 0; i < errors.size(); i++)
        {
            cout << ave_errors[i] << " ";
        }
        std::cout << std::endl;
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


/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

namespace po = boost::program_options;
void process_options(int argc, const char** argv)
{
    po::variables_map vm;

    try
    {
        po::options_description hidden_options("Hidden options");
        hidden_options.add_options()
            ("data-fname",
                 po::value<string>(&data_fname)->required(),
                 "The file that contains the data");
        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("exp-dir,E",
                 po::value<string>(&exp_dir)->required(),
                 "Output directory")
            ("error-mode,M",
                 po::value<string>(&err_mode)->required(),
                 "Error mode (rms, abs)")
            ("visualize", 
                 po::bool_switch(&visualize)->default_value(false),
                 "visualize the plots (including the testing part")
            ("vis-dir", 
                 po::value<string>(&vis_dir), "directory to save the plots")
            ("gp-scale", 
                 po::value<double>(&gp_scale)->default_value(10), 
                 "scale of gaussian process prior")
            ("gp-variance", 
                 po::value<double>(&gp_variance)->default_value(1.0), 
                 "signal variance of gaussian process prior")
            ("training-percent", 
                 po::value<double>(&training_percent)->default_value(0.8), 
                 "training percent in learning");

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("data-fname", -1);

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
