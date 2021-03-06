/* $Id$ */

#ifndef SLIC_ROTATION_INCLUDED
#define SLIC_ROTATION_INCLUDED

/* Start for c2man */

#include "m/m_incl.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int compute_rotation
(
    double angle,
    double d_C_w,
    double P_z ,
    int    width,
    int    height,
    Matrix **T_mpp
);

int compute_rotation_set_focal_length
(
    double angle,
    double d_C_w,
    double P_z ,
    double focal_length,
    int    width,
    int    height,
    Matrix **T_mpp
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

