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

/* $Id: test_ode_solver.cpp 20198 2015-12-20 15:56:27Z jguan1 $ */

#include <l_cpp/l_test.h>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/odeint.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/coupled_oscillator.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/adapter.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

void test_true(const State_vec_vec& states_1, const State_vec_vec& states_2)
{
    assert(states_1.size() == states_2.size());
    for(size_t i = 0; i < states_1.size(); i++)
    {
        for(size_t j = 0; j < states_1[i][0].size(); j++)
        {
            double v1 = states_1[i][0][j];
            double v2 = states_2[i][0][j];
            TEST_TRUE(fabs(v1 - v2) < 1e-5);
        }
    }
}

State_type get_init_state(size_t num_osc)
{
    State_type state(2 * num_osc);
    for(size_t i = 0; i < state.size(); i++)
    {
        state[i] = kjb_c::kjb_rand();
    }
    return state;
}

int main(int argc, const char** argv)
{
    string out_fp = "./outputs/";
    if(argc >= 2)
    {
        out_fp = argv[1];
    }
    ETX(kjb_c::kjb_mkdir(out_fp.c_str()));

    Double_v times(400);
    vector<size_t> time_indices(times.size());
    for(size_t i = 0; i < times.size(); i++)
    {
        times[i] = i * 1.0;
        time_indices[i] = i;
    }
    const size_t num_osc = 4;
    State_type init_state = get_init_state(num_osc);
    Double_v com_params = coupled_oscillator_params(num_osc);

    Vector obs_sigma(1, 0.5);

    std::vector<std::string> obs_names(1);
    obs_names[0] = "dial";
    int poly_degree = -1;
    Coupled_oscillator_v clos(times.size() - 1, Coupled_oscillator(com_params));
    Linear_state_space lss(times, 
                           init_state, 
                           clos, 
                           obs_names, 
                           obs_sigma, 
                           poly_degree);

    Lss_adapter lss_adapter(false, true);
    const double temp = 0.01;

    for(size_t i = 0; i < com_params.size(); i++)
    {
        Double_v new_params(com_params.begin(), com_params.end());
        new_params[i] += temp;
        Linear_state_space new_lss(times, 
                                  init_state, 
                                  clos,
                                  obs_names, 
                                  obs_sigma, 
                                  poly_degree);
        const Matrix& A = new_lss.coupled_oscillators().front().system_matrix();
        double old_param = lss_adapter.get(&lss, i);
        lss_adapter.set(&lss, i, old_param + temp);

        const Matrix& B = lss.coupled_oscillators().front().system_matrix();

        const State_vec_vec& states_1 = lss.get_states();
        const State_vec_vec& states_2 = new_lss.get_states();
        test_true(states_1, states_2);
        lss_adapter.set(&lss, i, old_param);
    }

    RETURN_VICTORIOUSLY();
}
