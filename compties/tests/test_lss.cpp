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

/* $Id: test_lss.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include <l_cpp/l_test.h>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/odeint.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/coupled_oscillator.h"
#include "dbn_cpp/util.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
    string out_fp = argv[1];
    if(argc >= 2)
    {
        out_fp = argv[1];
    }
    ETX(kjb_c::kjb_mkdir(out_fp.c_str()));

    Double_v times(60);
    for(size_t i = 0; i < times.size(); i++)
    {
        times[i] = i;
    }
    int num_osc = 2;
    size_t i = 0;
    //State_type init_state(6);
    State_type init_state(2*num_osc, 0.0);
    init_state[i++] = 2.26906184e-01;
    init_state[i++] = -1.96332753e-01; 
    //init_state[i++] = -1.5; 
    //init_state[i++] = 1.3; 
    init_state[i++] = 1.70979823e+00; 
    init_state[i++] = 5.05113839e-02;

    //Double_v com_params = coupled_oscillator_params(num_osc);
    i = 0;
    int num_params = param_length(num_osc);
    Double_v com_params(num_params, 0.0);
    // mass
    com_params[i++] = 0.12;
    com_params[i++] = 0.10;
    // stiffness
    com_params[i++] = 0.02; 
    com_params[i++] = 2.0;
    com_params[i++] = 0.02; 
    // damping
    com_params[i++] = 0.01;
    com_params[i++] = 0.02;

    std::vector<std::string> obs_names(1);
    obs_names[0] = "dial";
    //obs_names[1] = "ibi";
    //obs_names[1] = "hr";
    double noise_sigma = 0.5;
    //int poly_degree = 1;
    int poly_degree = -1;
    
    Coupled_oscillator_v clos(times.size() - 1, Coupled_oscillator(com_params));
    Linear_state_space lss(times, init_state, clos, obs_names, 
                           Vector(1, noise_sigma), poly_degree);

    if(poly_degree > -1)
    {
        for(size_t i = 0; i < lss.num_oscillators(); i++)
        {
            lss.set_polynomial_coefs(i, 0, 500);
        }
    }

    const Double_v& times_1 = lss.get_times();
    const Coupled_oscillator_v& clos_1 = lss.coupled_oscillators();
    const State_vec_vec& states_1 = lss.get_states();

    lss.write(out_fp);
    plot_model(lss, out_fp);
    return 0;

    Linear_state_space lss_2;
    lss_2.read(out_fp);
    cout << lss_2.allow_drift() << std::endl;
    const Double_v& times_2 = lss_2.get_times();
    const Coupled_oscillator_v& clos_2 = lss_2.coupled_oscillators();
    const State_vec_vec& states_2 = lss_2.get_states();


    TEST_TRUE(times_1.size() == times_2.size());
    TEST_TRUE(clos_1.size() == clos_2.size());
    TEST_TRUE(states_1.size() == states_2.size());
    for(size_t i = 0; i < states_1.size(); i++)
    {
        TEST_TRUE(times_1[i] == times_2[i]);
        for(size_t k = 0; k < states_1[i].size(); k++)
        {
            for(size_t j = 0; j < states_1[i][k].size(); j++)
            {
                TEST_TRUE(fabs(states_1[i][k][j] - states_2[i][k][j]) < 1e-5);
            }
        }
    }
    for(size_t i = 0; i < clos_1.size(); i++)
    {
        for(size_t j = 0; j < clos_1[i].num_params(); j++)
        {
            TEST_TRUE(fabs(clos_1[i].get_param(j) -  
                      clos_2[i].get_param(j)) < FLT_EPSILON);
        }
    }


    RETURN_VICTORIOUSLY();
}
