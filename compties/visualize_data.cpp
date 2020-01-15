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
#include <iostream>

#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/coupled_oscillator.h"
#include "dbn_cpp/util.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc != 4) 
    {
        std::cout << argv[0] << " " << " data-dir list-fp out-dir\n";
        return EXIT_SUCCESS;
    }
    std::string data_dir = argv[1];
    std::string list_fp = argv[2];
    std::string out_dir = argv[3];
    std::vector<Data> data = parse_data(data_dir, list_fp);
    for(size_t i = 0; i < data.size(); i++)
    {
        plot_data(data[i], out_dir);
    }
}
