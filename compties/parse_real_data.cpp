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

/* $Id: parse_real_data.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#include "l/l_sys_io.h"
#include "l/l_init.h"
#include <l_cpp/l_exception.h>

#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <ostream>
#include <iomanip>

#ifdef IVI_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/real_data_parser.h"
#include "dbn_cpp/util.h"

using namespace std;
using namespace ivi;
using namespace ivi::ties;

vector<string> moderators;
vector<string> observables;
vector<string> outcomes;
string input_file;
string outcome_file;
string output_dir;
string distinguisher;
string grouping_var;
size_t num_oscillators;
size_t average_size;

// percentage (from 0 to 100) of data to be excluded at the end
size_t exclude_percent;

bool standardize_data; 
bool mean_centered_data;
bool take_diff_mod;
bool take_ave_mod;

/** @brief options handler  */
void process_options(int argc, const char* argv[]);

int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif

    try
    {
        process_options(argc, argv);

        std::string out_dir("");
        if(grouping_var == "")
        {
            out_dir = output_dir;
            
        }
        Group_map group_map;
        std::vector<Data> all_data = parse_real_data(moderators, 
                                                     observables, 
                                                     input_file, 
                                                     distinguisher, 
                                                     group_map,
                                                     grouping_var,
                                                     num_oscillators,
                                                     take_diff_mod,
                                                     take_ave_mod);
        if(!outcomes.empty() && outcome_file != "")
        {
            parse_outcome_data(all_data, outcome_file, distinguisher, outcomes);
        }

        if(average_size > 1)
        {
            BOOST_FOREACH(Data& data, all_data)
            {
                size_t time_length = data.times.size();
                size_t new_length = 1.0 * time_length / average_size;
                data.times.resize(new_length);
                BOOST_FOREACH(Obs_map::value_type& value, data.observables)
                {
                    // copy the old values
                    Vector_v obs_vals = value.second;
                    for(size_t i = 0; i < obs_vals.size(); i++)
                    {
                        value.second[i].resize(new_length);
                        size_t cur_index = 0;
                        size_t new_index = 0;
                        while(cur_index < obs_vals[i].size())
                        {
                            size_t valid = 0;
                            double ave_value = 0.0;
                            for(size_t j = cur_index; 
                                    j < cur_index + average_size; 
                                    j++)
                            {
                                if(!invalid_data(obs_vals[i][j]))
                                {
                                    ave_value += obs_vals[i][j];
                                    valid++;
                                }
                            }
                            if(valid > 0) ave_value /= valid;
                            else ave_value = INT_MAX;
                            cur_index += average_size;
                            value.second[i][new_index++] = ave_value;
                        }
                    }
                }
            }
        }

        bool convert_mean = standardize_data || mean_centered_data ? true : false;
        bool convert_std = standardize_data ? true : false;

        if(standardize_data || mean_centered_data)
        {
            BOOST_FOREACH(const std::string& obs, observables)
            {
                BOOST_FOREACH(Data& data, all_data)
                {
                    std::pair<Vector, Vector> stats = 
                            standardize(data, obs, convert_mean, convert_std);
                    std::cout << obs << ": " << " mean: " << stats.first 
                                             << " variance: " << stats.second
                                             << std::endl;
                }
            }
        }

        // write out the data 
        if(grouping_var != "")
        {
            get_data_group(all_data, output_dir, group_map);
        }
        else
        {
            write_data(all_data, output_dir, exclude_percent);
        }

        if(!group_map.empty())
        {
            write_group(group_map, output_dir);
        }

        // write out outcome variables into one file
        if(!outcomes.empty())
        {
            std::string outcome_fp(output_dir + "/outcome.txt");
            std::ofstream ofs(outcome_fp.c_str());
            IFTD(ofs.is_open(), IO_error, "can't open file %s", 
                    (outcome_fp.c_str()));
            for(size_t i = 0; i < all_data.size(); i++)
            {
                assert(!all_data[i].outcomes.empty());
                ofs << all_data[i].dyid << " ";
                BOOST_FOREACH(const Mod_map::value_type& vm, all_data[i].outcomes)
                {
                    BOOST_FOREACH(double val, vm.second)
                    {
                        ofs << val << " "; 
                    }
                }
                ofs << std::endl;
            }
        }

        return EXIT_SUCCESS;
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
    catch(boost::exception& e)
    {
        cerr << "Uncaught exception: "
            << boost::diagnostic_information(e) << endl;
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
            ("out-dp,O",
                po::value<string>(&output_dir)->required(),
                "Output directory");

        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("input-file,I", 
            po::value<std::string>(&input_file)->required(), "input-file")
            ("moderator", 
             po::value<vector<string> >(&moderators)->multitoken(), 
            "moderators")
            ("observable", 
            po::value<vector<string> >(&observables)->multitoken(), 
            "observables")
            ("distinguisher",
                po::value<string>(&distinguisher)->required(),
                "distinguisher")
            ("categorical-moderator", po::value<string>(&grouping_var),
                "Variable that groups data together")
            ("num-oscillators", 
                po::value<size_t>(&num_oscillators)->default_value(2), 
                "number of oscillators(people) in a coupled group")
            ("average-size", 
                po::value<size_t>(&average_size)->default_value(1), 
                "number of data points to be averaged over")
            ("exclude-percent", 
                po::value<size_t>(&exclude_percent)->default_value(0), 
                "percentage ([0,100]) of data points to be excluded at the end")
            ("standardize-data", 
                po::bool_switch(&standardize_data)->default_value(false), 
                "if present standardize the data to have mean 0 and variance")
            ("moderator-diff", 
                po::bool_switch(&take_diff_mod)->default_value(false), 
                "if present taking the diff of the two oscillators")
            ("moderator-ave", 
                po::bool_switch(&take_ave_mod)->default_value(false), 
                "if present taking the average of the two oscillators")
            ("mean-centered-data", 
                po::bool_switch(&mean_centered_data)->default_value(false), 
                "if present center data to have mean 0")
            ("outcome", 
             po::value<vector<string> >(&outcomes)->multitoken(), 
             "outcome variables")
            ("outcome-csv-file",
                po::value<string>(&outcome_file), 
                "csv file contains the outcome variables");

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("out-dp", 1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);

        // help called?
        if(vm.count("help") || argc == 1)
        {
            cout << "usage:" << argv[0] << visible_options << " "
                             << hidden_options << "\n";

            exit(EXIT_SUCCESS);
        }

        // check to see if there is observable 
        if(!vm.count("observable"))
        {
            std::cerr << "No observable is provided. Exiting the program ...\n";
            exit(EXIT_SUCCESS);
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
