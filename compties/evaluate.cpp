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

/* $Id: evaluate.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include <l_cpp/l_exception.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#ifdef KJB_HAVE_BST_POPTIONS
#include <boost/program_options.hpp>
#else
#error "Need boost program options"
#endif
#include <boost/exception/diagnostic_information.hpp>
#include <boost/foreach.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/util.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

string data_dir;
string exp_dir;
string id_fp;
string vis_dir;
double training_percent;
bool visualize;
void process_options(int argc, const char** argv);

int main(int argc, const char** argv)
{
#ifdef TEST
    kjb_c::kjb_init();
    kjb_c::kjb_l_set("heap-checking", "off");
    kjb_c::kjb_l_set("initialization-checking", "off");
#endif
    try
    {
        // process options
        process_options(argc, argv);

        std::vector<size_t> ids = parse_list(id_fp);
        size_t num_lss = ids.size();

        // input file name 
        std::string err_fp_str = "rms_error.txt";
        std::string serr_fp_str = "sampled_rms_error.txt";
        boost::format data_fmt(data_dir + "/%04d.txt");
        boost::format lss_fmt(exp_dir + "/%04d/");
        bool has_sampled_err = false;

        // output error dir 
        std::string err_dp(exp_dir + "/errors/");
        ETX(kjb_c::kjb_mkdir(err_dp.c_str()));

        std::vector<Vector> all_ave_errors(num_lss);
        std::vector<std::vector<Vector> > all_obs_errors;
        std::vector<Vector> sampled_all_ave_errors(num_lss);
        std::vector<std::vector<Vector> > sampled_all_obs_errors;

        std::vector<std::string> obs_names;
        std::vector<Data> all_data;
        
        for(size_t i = 0; i < num_lss; i++)
        {
            string data_fname = (data_fmt % ids[i]).str();
            Data data = parse_data(data_fname);
            all_data.push_back(data);

            // read in the linear state 
            string lss_dir = (lss_fmt % ids[i]).str();
            Linear_state_space lss;
            lss.read(lss_dir);
            if(i == 0)
            {
                std::copy(lss.obs_names().begin(), lss.obs_names().end(), 
                        back_inserter(obs_names));
                all_obs_errors.resize(obs_names.size());
                sampled_all_obs_errors.resize(obs_names.size());
            }

            all_obs_errors.resize(lss.obs_names().size());

            // visualize 
            if(visualize)
            {
                if(vis_dir == "")
                {
                    vis_dir = lss_dir;
                    cerr << " visualize directory is not provided,"
                         << " saving to " << lss_dir << std::endl;
                }
                plot_data_and_model(data, lss, vis_dir, true);
            }

            // read in the error files 
            Vector ave_error;
            std::vector<Vector> obs_errors;
            read_couple_errors(lss_dir, err_fp_str, obs_names, 
                               ave_error, obs_errors);
            all_ave_errors[i] = ave_error;

            Vector save_error;
            std::vector<Vector> sobs_errors;
            string sample_dp(lss_dir + "/pred_samples");
            has_sampled_err = read_couple_errors(sample_dp, serr_fp_str, 
                                               obs_names, save_error, 
                                               sobs_errors);
            if(has_sampled_err)
            {
                sampled_all_ave_errors[i] = save_error;

                if(sobs_errors.size() > 1)
                {
                    for(size_t j = 0; j < sobs_errors.size(); j++)
                    {
                        sampled_all_obs_errors[j].push_back(sobs_errors[j]);
                    }
                }
            }
        }

        // output detailed errors 
        std::string ave_out_fp(err_dp + "/err_couples.txt");
        write_couple_error(ave_out_fp, all_ave_errors, all_data);

        if(has_sampled_err)
        {
            std::string save_out_fp(err_dp + "/sampled_err_couples.txt");
            write_couple_error(save_out_fp, sampled_all_ave_errors, all_data);
        }

        if(obs_names.size() > 1)
        {
            for(size_t i = 0; i < obs_names.size(); i++)
            {
                std::string obs_fp(err_dp + "/" + obs_names[i] 
                                          + "_err_couples.txt");
                write_couple_error(obs_fp, all_obs_errors[i], all_data);

                if(has_sampled_err)
                {
                    std::string sobs_fp(err_dp + "/" + obs_names[i] 
                                              + "_sampled_err_couples.txt");
                    write_couple_error(sobs_fp, 
                                       sampled_all_obs_errors[i], 
                                       all_data);
                }
            }
        }

        // write out the summary 
        std::string out_fp(err_dp + "/err_summary.txt");
        std::string out_latex_fp(err_dp + "/latex_err_summary.txt");
        compute_and_record_errors(all_ave_errors, string(""), out_fp);
        compute_and_record_errors_latex(all_ave_errors, out_latex_fp);
        if(obs_names.size() > 1)
        {
            for(size_t i = 0; i < obs_names.size(); i++)
            {
                std::string obs_fp(err_dp + "/" + obs_names[i] + "_err_summary.txt");
                std::string obs_latex_fp(err_dp + "/" + obs_names[i] 
                                + "_latex_err_summary.txt");
                compute_and_record_errors(all_obs_errors[i], string(""), obs_fp);
                compute_and_record_errors_latex(all_obs_errors[i], obs_latex_fp);
            }
        }

        if(has_sampled_err)
        {
            std::string sout_fp(err_dp + "/sampled_err_summary.txt");
            std::string sout_latex_fp(err_dp + "/latex_sampled_err_summary.txt");
            compute_and_record_errors(sampled_all_ave_errors, string(""), sout_fp);
            compute_and_record_errors_latex(sampled_all_ave_errors, sout_latex_fp);
            if(obs_names.size() > 1)
            {
                for(size_t i = 0; i < obs_names.size(); i++)
                {
                    std::string obs_fp(err_dp + "/" + obs_names[i] 
                                            + "_sampled_err_summary.txt");
                    std::string obs_latex_fp(err_dp + "/" + obs_names[i] 
                                    + "_latex_sampled_err_summary.txt");
                    compute_and_record_errors(sampled_all_obs_errors[i], string(""), obs_fp);
                    compute_and_record_errors_latex(sampled_all_obs_errors[i], 
                                                            obs_latex_fp);
                }
            }
        }
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
            ("data-dir",
                 po::value<string>(&data_dir)->required(),
                 "The file that contains the data");
        // General options
        po::options_description generic_options("General options");
        generic_options.add_options()
            ("help,h", "produce help message")
            ("exp-dir,E",
                 po::value<string>(&exp_dir)->required(),
                 "Experiment directory")
            ("id-list-fp,L",
                 po::value<string>(&id_fp)->required(),
                 "file that contains the id list")
            ("visualize", 
                 po::bool_switch(&visualize)->default_value(false),
                 "visualize the plots (including the testing part")
            ("vis-dir", 
                 po::value<string>(&vis_dir), "directory to save the plots")
            ("training-percent", 
                 po::value<double>(&training_percent)->default_value(0.8), 
                 "training percent in learning");

        // combine options
        po::options_description visible_options;
        visible_options.add(generic_options);

        po::options_description cmdline_options;
        cmdline_options.add(visible_options).add(hidden_options);

        po::positional_options_description pstnl;
        pstnl.add("data-dir", -1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                                .positional(pstnl).run(), vm);

        // help called?
        if(vm.count("help"))
        {
            cout << "usage: ./hmc data-file"
                      << visible_options << "\n";

            exit(EXIT_SUCCESS);
        }

        // open config file if it is given
        if(vm.count("config-file"))
        {
            string config_file_name = vm["config-file"].as<string>();
            ifstream config_fs(config_file_name.c_str());
            IFTD(config_fs.is_open(), IO_error, 
                    "Error opening configuration file: %s.",
                    (config_file_name.c_str()));

            po::store(po::parse_config_file(config_fs, visible_options), vm);
        }

        po::notify(vm);
    }
    catch(const po::error& err)
    {
        throw kjb::Exception(err.what());
    }    
    catch(const exception& ex)
    {
        throw ex;
    }    
}
