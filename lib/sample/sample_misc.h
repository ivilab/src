
/* $Id: sample_misc.h 25612 2020-07-06 22:17:23Z kobus $ */

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
* =========================================================================== */

#ifndef SAMPLE_MISC_INCLUDED
#define SAMPLE_MISC_INCLUDED

/* Start for c2man */


#include "m/m_incl.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int ivi_rand_int(int lb, int ub);

int sample_from_discrete_distribution(int* idx, const Vector* dist);

double sample_from_uniform_distribution(double a, double b);

int sample_random_unit_vector(Vector** v);

int poisson_rand(double lambda);

double gamma_pdf(double x, double alpha, double beta);

double log_gamma_pdf(double x, double alpha, double beta);

double sample_from_gamma_distribution
(
    double alpha,
    double beta,
    double a,
    double b
);

double sample_from_gamma_distribution_2(double alpha, double beta);

int pick_m_from_n(Int_vector ** m_indexes, int m, int n);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


