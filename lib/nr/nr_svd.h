
/* $Id: nr_svd.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef NR_SVD_INCLUDED
#define NR_SVD_INCLUDED


#include "m/m_gen.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int do_numerical_recipes_svd
(
    const Matrix* a_mp,
    Matrix**      u_mpp,
    Vector**      d_vpp,
    Matrix**      v_trans_mpp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

