#include <l/l_sys_io.h>
#include <l_cpp/l_exception.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/base_line_models.h"

using namespace kjb;
using namespace kjb::ties;
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

        average_model_fitting(data_dir, list_fp, output_dir, obs_str, 
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
