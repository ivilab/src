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

#include <string>
#include <fstream>
#include <iostream>
#include <boost/format.hpp>
#include "dbn_cpp/data.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc != 3)
    {
        std::cout << "Usuage: " << argv[0] << " input-fp out-dir\n";
        return EXIT_SUCCESS;
    }
    string in_fp = string(argv[1]);
    string out_dp = string(argv[2]);
    Data data;
    data = parse_data(in_fp);
    size_t dyid = data.dyid;

    boost::format out_fp_fmt(out_dp + "/%04d.txt");

    // write out the male coupled oscillator
    size_t male_dyid = dyid;// + 1000;
    string out_fp = (out_fp_fmt % male_dyid).str();
    Data male_data(male_dyid);
    male_data.times = data.times;
    male_data.moderators = data.moderators;

    Obs_map::const_iterator it = data.observables.begin();
    size_t osc_index = 0;
    male_data.observables["triad-obs"] = Vector_v(3);
    for(; it != data.observables.end(); it++)
    {
        male_data.observables["triad-obs"][osc_index++] = it->second[1];
    }
    write_data(male_data, out_fp);

    // write out the female coupled oscillator
    /*size_t female_dyid = dyid + 2000;
    out_fp = (out_fp_fmt % female_dyid).str();
    Data female_data(female_dyid);
    female_data.times = data.times;
    female_data.moderators = data.moderators;

    it = data.observables.begin();
    osc_index = 0;
    female_data.observables["triad-obs"] = Vector_v(3);
    for(; it != data.observables.end(); it++)
    {
        female_data.observables["triad-obs"][osc_index++] = it->second[1];
    }
    write_data(female_data, out_fp);*/

    return EXIT_SUCCESS;
}
