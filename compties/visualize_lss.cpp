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

/* $Id$ */

#include <l_cpp/l_test.h>

#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/odeint.hpp>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/coupled_oscillator.h"
#include "dbn_cpp/util.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc != 3) 
    {
        std::cout << argv[0] << " " << " indir outdir\n";
        return EXIT_SUCCESS;
    }
    std::string indir = argv[1];
    std::string outdir = argv[2];
    Linear_state_space lss;
    lss.read(indir); 
    lss.changed_index() = 0;
                           
    const Double_v& times_1 = lss.get_times();
    const Coupled_oscillator_v& clos_1 = lss.coupled_oscillators();
    const State_vec_vec& states_1 = lss.get_states();

    lss.write(outdir);
    plot_model(lss, outdir);

    RETURN_VICTORIOUSLY();
}
