
/* $Id: stat_test.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */


#ifndef STAT_TEST_INCLUDED
#define STAT_TEST_INCLUDED

/* Start for c2man */

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int welch_t_test_one_sided
(
    double  m1,
    double  m2,
    double  v1,
    double  v2,
    int     n1,
    int     n2,
    double* t_prime_ptr,
    int*    df_ptr,
    double* affect_size_ptr,
    double* p_value_ptr 
);


int welch_t_statistic
(
    double  u1,
    double  u2,
    double  v1,
    double  v2,
    int     n1,
    int     n2,
    double* t_prime_ptr,
    int*    df_ptr, 
    double* affect_size_ptr
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif    /*   #include this file            */


