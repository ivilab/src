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

/* $Id: test_ode_solver.cpp 20073 2015-11-13 04:37:55Z jguan1 $ */

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
    double freq_0 = -0.09;
    double freq_1 = -0.08;
    double damp_0 = -0.05;
    double damp_1 = -0.04;
    double coupling_01 = 0.01;
    double coupling_10 = -0.02;

    string out_fp = "./output/";
    if(argc >= 2)
    {
        out_fp = argv[1];
    }
    ETX(kjb_c::kjb_mkdir(out_fp.c_str()));

    Double_v times(70);
    for(size_t i = 0; i < times.size(); i++)
    {
        times[i] = i;
    }
    size_t i = 0;
    State_type init_state(4);
    init_state[i++] = 0.5; 
    init_state[i++] = 0.3; 
    init_state[i++] = 0.0; 
    init_state[i++] = 0.0;

    //Double_v com_params(12, 0.0);
    Double_v com_params(6, 0.0);
    i = 0;
    com_params[i++] = freq_0; 
    com_params[i++] = damp_0; 
    com_params[i++] = coupling_01;
    com_params[i++] = freq_1;
    com_params[i++] = damp_1;
    com_params[i++] = coupling_10;

    Vector obs_sigma(1, 0.5);

    std::vector<std::string> obs_names(1);
    obs_names[0] = "dial";
    int poly_degree = -1;
    Coupled_oscillator_v init_clos(times.size() - 1, 
                                   Coupled_oscillator(com_params));
    Linear_state_space lss(times, 
                           init_state, 
                           init_clos,
                           obs_names, 
                           obs_sigma, 
                           poly_degree);
    Coupled_oscillator_v& clos = lss.coupled_oscillators();

    BOOST_FOREACH(Coupled_oscillator& clo, clos)
    {
        clo.set_param(0, -0.5);
        clo.set_param(1, -0.01);
        clo.set_param(2, -0.05);
        clo.set_param(3, -0.002);
        clo.set_param(4, 0.003);
        clo.set_param(5, 0.005);
    }
    lss.changed_index() = 0;
    const State_vec_vec& states_1 = lss.get_states();
    lss.write("output/lss1");
    com_params[0] = -0.5;
    com_params[1] = -0.01;
    com_params[2] = -0.05;
    com_params[3] = -0.002;
    com_params[4] = 0.003;
    com_params[5] = 0.005;
    Coupled_oscillator_v init_clos_2(times.size() - 1, Coupled_oscillator(com_params));
    Linear_state_space lss2(times, 
                           init_state, 
                           init_clos_2, 
                           obs_names, 
                           obs_sigma, 
                           poly_degree);

    std::cout << "lss2:\n " 
        << lss2.coupled_oscillators()[0].system_matrix() << std::endl;
    const State_vec_vec& states_2 = lss2.get_states();
    lss2.write("output/lss2");

    for(size_t i = 0; i < times.size(); i++)
    {
        for(size_t j = 0; j < states_1[i][0].size(); j++)
        {
            double v1 = states_1[i][0][j];
            double v2 = states_2[i][0][j];
            TEST_TRUE(fabs(v1 - v2) < 1e-5);
        }
    }


    RETURN_VICTORIOUSLY();
}
