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

/* $Id: test_lss_2.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <m_cpp/m_vector.h>
#include <p/p_plot.h>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>

#include "dbn_cpp/linear_state_space.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
    double freq_w = -0.09;
    double freq_m = -0.08;
    double damp_w = -0.05;
    double damp_m = -0.04;
    double coupling_w = 0.01;
    double coupling_m = -0.01;

    Matrix cur_values(1, 4);
    cur_values(0, 0) = 0.5; cur_values(0, 1) = 0.5; 
    cur_values(0, 2) = 0.0; cur_values(0, 3) = 0.0;

    ivi_c::set_max_num_plots(6);
    const double starting_time = 0.0;
    const double ending_time = 100.0;
    Double_v times;
    double t = starting_time;
    while(t <= ending_time)
    {
        double time = t;
        times.push_back(time);
        t++;
    }

    for(size_t i = 0; i < cur_values.get_num_rows(); i++)
    {
        int id = ivi_c::plot_open();
        ivi_c::set_colour_plot();
        ivi_c::plot_set_range(id, starting_time, ending_time, -1.5, 1.5);
        for(size_t j = 0; j < 1; j++)
        {
            if(j == 0)
            {
                freq_w = -0.07;
                freq_m = -0.07;
                damp_w = 0.0;
                damp_m = 0.0;
                coupling_w = 0.0;
                coupling_m = 0.0;
            }
            else if(j == 1)
            {
                freq_w = -0.08;
                freq_m = -0.25;
                damp_w = 0.16;
                damp_m = -0.28;
                coupling_w =  0.01;
                coupling_m = -0.13;
            }
            else if(j == 2)
            {
                freq_w = -0.14;
                freq_m = -0.09;
                damp_w = 0.05;
                damp_m = -0.02;
                coupling_w =  -0.02;
                coupling_m = 0.001;
            }

            Double_v com_params(6, 0.0);
            com_params[0] = freq_w; 
            com_params[1] = damp_w;
            com_params[2] = coupling_w; 
            com_params[3] = freq_m;
            com_params[4] = damp_m;
            com_params[5] = coupling_m;

            State_type init_state(4);
            init_state[0] = cur_values.get_row(i)[0];
            init_state[1] = cur_values.get_row(i)[1];
            init_state[2] = cur_values.get_row(i)[2];
            init_state[3] = cur_values.get_row(i)[3];


            std::vector<std::string> obs_names; 
            obs_names.push_back("dial");
            Vector noise_sigma(1, 0.01);
            int poly_degree = 1;
            int regime = -1;
            std::vector<std::string> outcomes;
            Coupled_oscillator_v clos(times.size() - 1, Coupled_oscillator(com_params));
            Linear_state_space lss(times, 
                                   init_state, 
                                   clos, 
                                   obs_names, 
                                   noise_sigma, 
                                   poly_degree, 
                                   outcomes);
            if(poly_degree > 0)
            {
                for(size_t i = 0; i < lss.polynomial_coefs().size(); i++)
                {
                    for(size_t j = 0; j < lss.polynomial_coefs()[i].size(); j++)
                    {
                        lss.set_polynomial_coefs(i, j, ivi_c::ivi_rand());
                    }
                }
            }
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
            string legend_name = "type" + boost::lexical_cast<string>(j) + " women";
            ivi_c::plot_curve(id, Vector(times.begin(), times.end()).get_c_vector(), 
                              w_y_values.get_c_vector(), legend_name.c_str());
            legend_name = "type" + boost::lexical_cast<string>(j) + " man";
            ivi_c::plot_curve(id, Vector(times.begin(), times.end()).get_c_vector(), 
                              m_y_values.get_c_vector(), legend_name.c_str());
            lss.write("outputs/test_2/");
        }
        string figure_name = "outputs/Figure_" + boost::lexical_cast<string>(i+1) + ".ps";
        ivi_c::save_plot(id, figure_name.c_str());
    }

    return EXIT_SUCCESS;
}
