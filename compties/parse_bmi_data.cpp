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

/* $Id: parse_bmi_data.cpp 22561 2019-06-09 00:05:52Z kobus $ */

#include "l/l_sys_io.h"
#include <l_cpp/l_exception.h>

#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>


#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <ostream>
#include <iomanip>
#include <iostream>

#include "dbn_cpp/data.h"

using namespace std;
using namespace kjb;
using namespace kjb::ties;



int main(int argc, const char** argv)
{
    if(argc != 4)
    {
        cout << "Usage: " << argv[0] << " input-file individual-file-dir output-dir\n";
        return EXIT_SUCCESS;
    }

    string in_fname = argv[1];
    string indv_dir = argv[2];
    string output_dir = argv[3];

    ifstream ifs(in_fname.c_str());
    IFTD(ifs.is_open(), IO_error, "Error opening input file: %s.\n", 
            (in_fname.c_str()));

    map<size_t, Data> info;
    map<size_t, double> w_bmi;
    map<size_t, double> m_bmi;
    size_t min_id = 1;
    size_t max_id = 46;
    // Parse in old data to get BMI_diff and BMI_ave values
    boost::format data_fmt(indv_dir + "/%04d.txt");
    for(size_t i = min_id; i <= max_id; i++)
    {
        string fname = (data_fmt % i).str();
        if(kjb_c::is_file(fname.c_str()))
        {
            Data data = parse_data(fname);
            // clear out the dial values
            data.observables.clear();
            data.times.clear();
            size_t dyid = data.dyid;
            info.insert(pair<size_t, Data>(dyid, data));
        }
    }

    // start parsing input file
    string line; 
    // get the first line (header info)
    getline(ifs,line);
    //const size_t num_tokens = 18;
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    while(getline(ifs, line))
    {
        vector<string> tokens;
        boost::trim(line);
        //boost::split(tokens, line, boost::is_any_of(", \"\\"), boost::token_compress_on);
        Tokenizer tok(line);
        tokens.assign(tok.begin(),tok.end());
        assert(tokens.size() == 46);
        // Parse line 
        
        size_t cur_id = boost::lexical_cast<size_t>(tokens[0]);
        size_t cur_dyid = boost::lexical_cast<size_t>(tokens[1]);
        map<size_t, Data>::iterator it = info.find(cur_dyid);
        if(it == info.end()) continue;

        // check to see if the current couple in the data
        int sex_m = boost::lexical_cast<int>(tokens[13]);
        int sex_f = boost::lexical_cast<int>(tokens[14]);
        double bmi = boost::lexical_cast<double>(tokens[10]); 
        double p_bmi = boost::lexical_cast<double>(tokens[11]);
        if(sex_m == 0 )
        {
            assert(sex_f == 1);
            w_bmi[cur_dyid] = bmi;
        }
        else
        {
            assert(sex_m == 1);
            m_bmi[cur_dyid] = bmi;
        }

    }
   
    boost::format out_fmt(output_dir + "/%04d.txt");
    ETX(kjb_c::kjb_mkdir(output_dir.c_str()));
    map<size_t, Data>::const_iterator it = info.begin();
    for(; it != info.end(); it++)
    {
        const Data& data = it->second;
        size_t dyid = data.dyid;
        map<size_t, double>::const_iterator w_bmi_p = w_bmi.find(dyid);
        map<size_t, double>::const_iterator m_bmi_p = m_bmi.find(dyid);
        assert(w_bmi_p != w_bmi.end() && m_bmi_p != m_bmi.end());
        double bmi_ave = (w_bmi_p->second + m_bmi_p->second)/2.0;

        string out_fname = (out_fmt % dyid).str();
        ofstream ofs(out_fname.c_str());
        IFTD(ofs.is_open(), IO_error, "Error opening output file: %s.\n", 
                (out_fname.c_str()));

        ofs << setw(6) << dyid 
            << setw(20) << setprecision(12) << w_bmi_p->second
            << setw(20) << setprecision(12) << m_bmi_p->second
            << std::endl;
    }

    return EXIT_SUCCESS;
}

