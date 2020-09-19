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

/* $Id: line_fitting.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <m_cpp/m_vector.h>
#include <g_cpp/g_line.h>
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

#include "dbn_cpp/data.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/base_line_models.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc < 6 || argc > 7)
    {
        std::cout << " Usage: " << argv[0] << " data-dir couple-list"
                     " output_dir observable distinguisher [training-percent(=0.8)]\n";
        return EXIT_SUCCESS;
    }

    try
    {
        string data_dir = argv[1];
        string list_fp = argv[2];
        string output_dir = argv[3];
        string obs_str = argv[4];
        string distinguisher = argv[5];
        double training_percent = 0.8;
        if(argc == 7)
        {
            training_percent = boost::lexical_cast<double>(argv[6]);
        }

        line_model_fitting(data_dir, list_fp, output_dir, obs_str, 
                           distinguisher, training_percent);

    } 
    catch(IO_error& e)
    {
        std::cerr << e.get_msg() << endl;
        exit(1);
    }
    catch(Exception& e)
    {
        std::cerr << "Uncaught exception: " << e.get_msg() << endl;
        exit(1);
    }
    catch(std::exception& e)
    {
        std::cerr << "Uncaught exception: " << e.what() << endl;
        exit(1);
    }
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }
    return EXIT_SUCCESS;
}

