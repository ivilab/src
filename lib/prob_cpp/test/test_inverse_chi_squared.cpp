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

/* $Id: test_inverse_chi_squared.cpp 25499 2020-06-14 13:26:04Z kobus $ */

#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_pdf.h>
#include <prob_cpp/prob_sample.h>
#include <l_cpp/l_test.h>

using namespace ivi;

int main(int argc, char** argv)
{
    const double dof = 10.0;
    const double scale = 4.0;

    Inverse_chi_squared_distribution ics(dof, scale);
    const size_t num_samples = 1e6;
    double sample_mean = 0.0;
    for(size_t i = 0; i < num_samples; i++)
    {
        double x = sample(ics);
        TEST_TRUE(fabs(pdf(ics, x) - exp(log_pdf(ics, x))) < FLT_EPSILON);
        sample_mean += x;
    }
    sample_mean /= num_samples;
    double real_mean = dof * scale /(dof - 2);
    //std::cout << sample_mean << " " << real_mean << std::endl;
    TEST_TRUE(fabs(sample_mean - real_mean) < 1e-3);

    return EXIT_SUCCESS;

}
