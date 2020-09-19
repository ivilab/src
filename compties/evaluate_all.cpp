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

/* $Id: evaluate_all.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include <l_cpp/l_exception.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>
#include <boost/foreach.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/util.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

string err_fname;
string exp_dir;
string out_fname;
string latex_fname;
double training_percent;
void process_options(int argc, const char** argv);

int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif
    try
    {
        // process options
        process_options(argc, argv);
        std::string err_fp(exp_dir + "/errors/" + err_fname);
        std::ifstream err_ifs(err_fp.c_str());
        IFTD(err_ifs.is_open(), IO_error,
                "Can't open file %s", (err_fp.c_str()));
        std::vector<Vector> errors;
        std::string line;
        getline(err_ifs, line);
        std::vector<double> first_elems;
        istringstream first_ist(line);
        std::copy(istream_iterator<double>(first_ist), istream_iterator<double>(), 
                back_inserter(first_elems));
        Vector error(first_elems.begin() + 1, first_elems.end());
        errors.push_back(error);
        while(getline(err_ifs, line))
        {
            std::vector<double> elems;
            std::istringstream ist(line);
            std::copy(istream_iterator<double>(ist), istream_iterator<double>(), 
                    back_inserter(elems));
            std::vector<double>::iterator eit= elems.begin();
            std::vector<double> error(eit+1, elems.end());
            Vector temp(error);
            errors.push_back(temp);
        }

        std::string out_fp(exp_dir + "/errors/" + out_fname);
        std::string latex_fp(exp_dir + "/errors/" + latex_fname);
        compute_and_record_errors(errors, string(""),  out_fp);
        compute_and_record_errors_latex(errors, latex_fp);

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
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }
    return EXIT_SUCCESS;
}


/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

namespace po = boost::program_options;
void process_options(int argc, const char** argv)
{
    po::variables_map vm;

    try
    {
        po::options_description hidden_options("Hidden options");
        hidden_options.add_options()
            ("err-fname",
                 po::value<string>(&err_fname)->required(),
                 "The file that contains the error");
        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("exp-dir,E",
                 po::value<string>(&exp_dir)->required(),
                 "Experiment directory")
            ("out-fname,F",
                 po::value<string>(&out_fname)->required(),
                 "output filename")
            ("latex-out-fname,X",
                 po::value<string>(&latex_fname)->required(),
                 "output filename");

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("err-fname", -1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);

        // help called?
        if(vm.count("help"))
        {
            cout << "usage: ./hmc err-fname"
                      << visible_options << "\n";

            exit(EXIT_SUCCESS);
        }

        // open config file if it is given
        if(vm.count("config-file"))
        {
            string config_file_name = vm["config-file"].as<string>();
            std::ifstream config_fs(config_file_name.c_str());
            IFTD(config_fs.is_open(), IO_error, 
                    "Error opening configuration file: %s.",
                    (config_file_name.c_str()));

            po::store(po::parse_config_file(config_fs, visible_options), vm);
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
