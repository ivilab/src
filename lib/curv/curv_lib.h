
/* $Id: curv_lib.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               |
|        Kobus Barnard                                                         |
|        Amy Platt                                                             |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef CURV_LIB_INCLUDED
#define CURV_LIB_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int thin_pixels_not_needed_for_contiguity(Int_matrix* image_mp);

int count_neighbors
(
    const Int_matrix* image_mp,
    int               i,
    int               j
);

int fit_parametric_cubic_known_time
(
    const Vector* t_vp, 
    const Matrix* data_mp,
    const Vector* weight_vp,
    Matrix** new_c_mpp,
    double* error_ptr
);

int fit_parametric_cubic
(
    const Vector* t_vp,
    const Matrix* data_mp,
    const Vector* weight_vp,
    const Matrix* c_mp,
    Vector**      new_t_vpp,
    Matrix**      new_c_mpp,
    double*       error_ptr
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

