
/* $Id: i_matrix.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef I_MATRIX_INCLUDED
#define I_MATRIX_INCLUDED


#include "l/l_def.h"
#include "m/m_gen.h"
#include "i/i_type.h"
#include "i/i_float.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int matrix_vector_to_image(const Matrix_vector* mvp, IVI_image** out_ipp);
int image_to_matrix_vector(const IVI_image* ip, Matrix_vector** mvpp);

int rgb_matrix_array_to_image
(
    Matrix*     mp_list[ 3 ],
    IVI_image** out_ip_arg
);

int image_to_rgb_matrix_array(const IVI_image* ip, Matrix* mp_list[ 3 ]);

int matrix_to_bw_image
(
    const Matrix* mp,
    IVI_image**   out_ipp
);

int matrix_to_max_contrast_8bit_bw_image
(
    const Matrix* mp,
    IVI_image**   out_ipp
);

int rgb_matrices_to_image
(
    const Matrix* r_mp,
    const Matrix* g_mp,
    const Matrix* b_mp,
    IVI_image**   out_ipp
);

int bw_image_to_matrix
(
    const IVI_image* ip,
    Matrix**         mpp
);

int image_to_matrix
(
    const IVI_image* ip,
    Matrix**         mpp
);

int image_to_matrix_2
(
    const IVI_image* ip,
    double           r_weight,
    double           g_weight,
    double           b_weight,
    Matrix**         mpp
);


int image_to_rgb_matrices
(
    const IVI_image* ip,
    Matrix**         r_mpp,
    Matrix**         g_mpp,
    Matrix**         b_mpp
);


void free_rgb_matrices(Matrix* r_mp, Matrix* g_mp, Matrix* b_mp);

void free_rgb_matrix_array(Matrix* mp_list[ 3 ]);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

