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

/* $Id: data_stat.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include <l/l_sys_io.h>
#include <l_cpp/l_exception.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <numeric>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc != 5)
    {
        std::cout << " Usuage: " << argv[0] << " data-dir "
                  << " list-fp out-dir observation\n";
        return EXIT_SUCCESS;
    }

    try{

        string data_dir = argv[1];
        string list_fp = argv[2];
        string out_dir = argv[3];
        string obs_str = argv[4];
        boost::format data_fmt(data_dir + "/%04d.txt");
        boost::format out_fmt(out_dir + "/%04d.txt");
        std::vector<size_t> ids = parse_list(list_fp);

        std::vector<Data> data;
        for(size_t i = 0; i < ids.size(); i++)
        {
            string fname = (data_fmt % ids[i]).str();
            data.push_back(parse_data(fname));
        }

        for(size_t t = 0; t < data.size(); t++)
        {
            Obs_map::const_iterator it = data[t].observables.begin();
            for(; it != data[t].observables.end(); it++)
            {
                //std::pair<Vector, Vector> stat = standardize(data[t], it->first, true, true);
                std::pair<Vector, Vector> stat = standardize(data[t], it->first, true, false);
                std::cout << data[t].dyid << " mean: " << stat.first << " ";
                std::cout << " variance: " << stat.second << std::endl;
            }
            std::cout << std::endl;
            string out_fp = (out_fmt % data[t].dyid).str();
            write_data(data[t], out_fp);
        }
        std::pair<Vector, Vector> stats = standardize(data, obs_str, false, false);
        std::cout << " mean: " << stats.first << " ";
        std::cout << " variance: " << stats.second << std::endl;
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

