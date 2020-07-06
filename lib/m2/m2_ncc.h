
/* $Id: m2_ncc.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *|
|  Copyright (c) 1994-2008 by Kobus Barnard & Quanfu Fan(author).
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

#ifndef M2_NCC_INCLUDED
#define M2_NCC_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/*
 * Ideally such a constant would not be defined here, but it has to stay for now
 * because it is used in test/ncc.c.
*/
#define NCC_EPSLON 1e-6


int fourier_convolve_matrix
(
    Matrix**      out_mpp,
    const Matrix* in_mp,
    const Matrix* mask_mp
);

int fourier_convolve_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Matrix* mask_mp
);

int ncc_matrix
(
    double*       corr,
    const Matrix* mp1,
    const Matrix* mp2
);

int ncc_mvector
(
    double*              corr,
    const Matrix_vector* mvp1,
    const Matrix_vector* mvp2
);

int fourier_ncc_template_matrix
(
    Matrix**      out_mpp,
    const Matrix* in_mp,
    const Matrix* mask_mp
);

int fourier_ncc_template_mvector
(
    Matrix**             out_mpp,
    const Matrix_vector* in_mvp,
    const Matrix_vector* mask_mvp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif


/* End for c2man */

#endif   /* Include this file. */

