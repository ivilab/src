/* =========================================================================== *
   |
   |  Copyright (c) 1994-2011 by Kobus Barnard (author)
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
   |  Author:  Jinyan Guan
 * =========================================================================== */

/* $Id$ */

/* @file This file is used to generate the model name based on the options */

#include "dbn_cpp/experiment.h"
#include "dbn_cpp/options.h"

using namespace kjb::ties;

int main(int argc, const char** argv)
{
    Ties_experiment exp = experiment_from_cl_options(argc, argv);
    generate_model_name(exp);
    std::cout << exp.model_name << std::endl;
    return EXIT_SUCCESS;
}
