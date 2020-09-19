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

/* $Id: test_lss_2.cpp 20073 2015-11-13 04:37:55Z jguan1 $ */

#include <m_cpp/m_vector.h>
#include <p/p_plot.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>
#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif

#include "dbn_cpp/linear_state_space.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

double freq_mean;
double freq_sigma;
double damping_mean;
double damping_sigma;
double coupling_mean;
double coupling_sigma;
size_t num_samples;
std::string out_dp;


/** @brief options handler  */
void process_options(int argc, const char** argv);

int main(int argc, const char** argv)
{
    try
    {
        process_options(argc, argv);

        State_type init_state(4);
        init_state[0] = 0.5;
        init_state[1] = 0.3;;
        init_state[2] = 0.0;
        init_state[3] = 0.0;

        Double_v times(100);
        for(size_t i = 0; i < times.size(); i++)
        {
            times[i] = i * 1.0;
        }

        string lss_out_dp(out_dp + "/lss/");
        string plot_out_dp(out_dp + "/plot/");
        boost::format lss_out_fmt(lss_out_dp + "/%04d/");
        boost::format plot_out_fmt(plot_out_dp + "/%04d.ps");
        ETX(ivi_c::ivi_mkdir(lss_out_dp.c_str()));
        ETX(ivi_c::ivi_mkdir(plot_out_dp.c_str()));

        double init_mass = -0.02;
        double last_coupling = 0.02;
        double step = (last_coupling - init_mass)/num_samples;
        for(size_t i = 0; i < num_samples; i++)
        {
            int id = ivi_c::plot_open();
            ivi_c::set_colour_plot();
            ivi_c::plot_set_range(id, times.front(), times.back(), -5.0, 5.0);

            Double_v com_params(6, 0.0);
            // frequency
            com_params[0] = -0.09; 
            // damping
            com_params[1] = -0.08;
            // coupling
            com_params[2] = (init_mass + i * step); 
            // frequency
            com_params[3] = -0.08;
            // damping
            com_params[4] = -0.08; 
            // coupling
            com_params[5] = -com_params[2];


            std::vector<std::string> obs_names; 
            obs_names.push_back("dial");
            Coupled_oscillator_v clos(times.size() - 1, Coupled_oscillator(com_params));
            Linear_state_space lss(times, init_state, clos, obs_names);
            const State_vec_vec& values = lss.get_states();

            // Plot 
            // create y vectors
            Vector w_y_values((int)values.size(), 0.0);
            Vector m_y_values((int)values.size(), 0.0);
            for(size_t k = 0; k < times.size(); k++)
            {
                w_y_values[k] = values[k][0][0];
                m_y_values[k] = values[k][0][1];
            }
            string legend_name = "women " + boost::lexical_cast<string>(com_params[0]) + " "  
                                          + boost::lexical_cast<string>(com_params[1]) + " "
                                          + boost::lexical_cast<string>(com_params[2]);
            ivi_c::plot_curve(id, Vector(times.begin(), times.end()).get_c_vector(), 
                              w_y_values.get_c_vector(), legend_name.c_str());
            legend_name = "men " + boost::lexical_cast<string>(com_params[3]) + " "  
                                 + boost::lexical_cast<string>(com_params[4]) + " "
                                 + boost::lexical_cast<string>(com_params[5]);
            ivi_c::plot_curve(id, Vector(times.begin(), times.end()).get_c_vector(), 
                              m_y_values.get_c_vector(), legend_name.c_str());
            std::string out_fp = (lss_out_fmt % i).str();
            lss.write(out_fp);
            string figure_name = (plot_out_fmt % i).str();
            ivi_c::save_plot(id, figure_name.c_str());
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

namespace po = boost::program_options;
void process_options(int argc, const char** argv)
{
    po::variables_map vm;

    try
    {
        po::options_description hidden_options("Hidden options");
        hidden_options.add_options()
            ("output-dir",
                 po::value<string>(&out_dp)->required(),
                 "Output directory");
        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("num-samples,N",
                 po::value<size_t>(&num_samples)->default_value(1),
                 "Number of threads of the gradient computation")
            ("freq-mean", 
                 po::value<double>(&freq_mean)->default_value(0.0001),
                 "mean of frequency")
            ("freq-sigma", 
                 po::value<double>(&freq_sigma)->default_value(0.0001),
                 "sigma of frequency")
            ("damping-mean", 
                 po::value<double>(&damping_mean)->default_value(0.0001),
                 "mean of dampinguency")
            ("damping-sigma", 
                 po::value<double>(&damping_sigma)->default_value(0.0001),
                 "sigma of dampinguency")
            ("coupling-mean", 
                 po::value<double>(&coupling_mean)->default_value(0.0001),
                 "mean of couplinguency")
            ("coupling-sigma", 
                 po::value<double>(&coupling_sigma)->default_value(0.0001),
                 "sigma of couplinguency");

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("output-dir", 1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);

        // help called?
        if(vm.count("help"))
        {
            cout << "usage: " << argv[0] << hidden_options 
                      << visible_options << "\n";

            exit(EXIT_SUCCESS);
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
