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

/* $Id: split_data.cpp 25797 2020-09-19 21:14:43Z kobus $ */

#ifndef IVI_HAVE_ERGO
#error "You need libergo to use this program"
#endif

#include "dbn_cpp/data.h"
#include "dbn_cpp/likelihood.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>


using namespace ivi;
using namespace ivi::ties;
using namespace std;


int main(int argc, const char** argv)
{
#ifdef TEST
    ivi_c::ivi_init();
    ivi_c::ivi_l_set("heap-checking", "off");
    ivi_c::ivi_l_set("initialization-checking", "off");
#endif
    if(argc < 3)
    {
        std::cout << " Usuage: " << argv[0] << " list-fp out-dir\n";
        return EXIT_SUCCESS;
    }

    string list_fp = argv[1];
    string out_dp = argv[2];
    int seed = 0;
    if(argc >= 4)
        seed = atoi(argv[3]);
    ETX(ivi_c::ivi_mkdir(out_dp.c_str()));
    //seed_sampling_rand(seed);
    ivi_c::ivi_seed_rand(seed, seed);
    ivi_c::ivi_seed_rand_2(seed);
    srand(seed);

    vector<size_t> ids = parse_list(list_fp);
    random_shuffle(ids.begin(), ids.end());

    string training_fp(out_dp + "/training-list.txt");
    string testing_fp(out_dp + "/testing-list.txt");
    ofstream training_ofs(training_fp.c_str());
    ofstream testing_ofs(testing_fp.c_str());
    IFTD(!training_ofs.fail(), IO_error, "can't open file %s", 
            (training_fp.c_str()));
    IFTD(!testing_ofs.fail(), IO_error, "can't open file %s", 
            (testing_fp.c_str()));

    for(size_t i = 0; i < 30; i++)
    {
        training_ofs << ids[i] << endl;
    }
    for(size_t i = 30; i < ids.size(); i++)
    {
        testing_ofs << ids[i] << endl;
    }

    return EXIT_SUCCESS;
}
