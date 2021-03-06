
/* $Id: x_matrix.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 2003-2008 by Kobus Barnard (author).
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

#ifndef X_MATRIX_INCLUDED
#define X_MATRIX_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int complex_multiply_matrices_ew
(
    Matrix**      out_re_mpp,
    Matrix**      out_im_mpp,
    const Matrix* in_re1_mp,
    const Matrix* in_im1_mp,
    const Matrix* in_re2_mp,
    const Matrix* in_im2_mp
);

int complex_divide_matrices_ew
(
    Matrix**      out_re_mpp,
    Matrix**      out_im_mpp,
    const Matrix* in_re1_mp,
    const Matrix* in_im1_mp,
    const Matrix* in_re2_mp,
    const Matrix* in_im2_mp
);

int complex_get_magnitude_matrix_ew
(
    Matrix**      out_mpp,
    const Matrix* in_re_mp,
    const Matrix* in_im_mp
);

int complex_get_phase_matrix_ew
(
    Matrix**      out_mpp,
    const Matrix* in_re_mp,
    const Matrix* in_im_mp
);

int complex_multiply_matrices
(
    Matrix**      out_re_mpp,
    Matrix**      out_im_mpp,
    const Matrix* in_re1_mp,
    const Matrix* in_im1_mp,
    const Matrix* in_re2_mp,
    const Matrix* in_im2_mp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


