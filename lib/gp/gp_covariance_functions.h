
/* $Id: gp_covariance_functions.h 25499 2020-06-14 13:26:04Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|        Ernesto Brau                                                          |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef GP_COVARIANCE_FUNCTIONS_H_INCLUDED
#define GP_COVARIANCE_FUNCTIONS_H_INCLUDED


#include "m/m_incl.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int squared_exponential_covariance_function
(
    Matrix**        cov,
    const Vector*   t1,
    const Vector*   t2,
    const void*     l,
    int             d
);

int zero_mean_function(Vector** mn, const Vector* t, int d);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif /* COVARIANCE_FUNCTIONS_H_ */

