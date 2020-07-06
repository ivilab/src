/* $Id: affine.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_AFFINE_INCLUDED
#define SLIC_AFFINE_INCLUDED

/* Start for c2man */

#include "slic/basic.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int fit_affine
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    Matrix       **a_mpp,
    double       *fit_err_ptr
);

int affine_inverse
(
    const Matrix *a_mp,
    const Matrix *mp,
    Matrix       **res_mpp
);

int affine_transform_single_point
(
    const Matrix *a_mp,
    const Vector *x_vp,
    Vector       **y_vpp
);

int affine_transform
(
    const Matrix *a_mp,
    const Matrix *x_mp,
    Matrix       **y_mpp
);

int get_affine_fitting_error
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *a_mp,
    double *fit_err_ptr
);
        
int get_affine_distance
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *a_mp, 
    Vector       **dist_vpp
);

/* 
   Ernesto added a struct in n/n_cholesky_decomposition
   So I renamed this to a more specific struct name.
 */
int cholesky_decomposition_SLIC
(
    const Matrix *mp,
    Matrix       **res_mpp
);

int affine_decomposition
(
    const Matrix *a_mp,
    Matrix       **R_mpp,
    Matrix       **S_mpp,
    Matrix       **Z_mpp
);

int is_affine_degenerate
(
    const Matrix *x_mp
);

int constrained_affine_1
(
    const Matrix *a_mp
);

int constraint_affine_2
(
    const Matrix *a_mp
);


int get_color_constancy_matrix
(
    const IVI_image *src_img,
    const IVI_image *target_img,
    const Int_matrix *mask_imp,
    Matrix **color_constancy_mpp
);

int apply_color_constancy
(
    const IVI_image *src_img,
    const Int_matrix *mask_imp,
    const Matrix *color_constancy_mp,
    IVI_image       **res_img
);

int do_color_constancy
(
    const IVI_image *src_img,
    const IVI_image *target_img,
    const Int_matrix *mask_imp,
    IVI_image       **res_img
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
