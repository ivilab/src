/* $Id: homography.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_HOMOGRAPHY_INCLUDED
#define SLIC_HOMOGRAPHY_INCLUDED

/* Start for c2man */

#include "slic/basic.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int fit_homography
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    Matrix       **H_mpp,
    double       *fit_err_ptr
);

int normalize_2Dpoints
(
    const Matrix *pts_mp,
    Matrix       **new_pts_mpp,
    Matrix       **trans_mpp
);

int is_homography_degenerate
(
    const Matrix *x_mp
);

int get_homography_distance
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *H_mp, 
    Vector       **dist_vpp
);

int get_dual_homography_distance
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *H_mp, 
    Vector       **dist_vpp
);

int get_homography_fitting_error
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *H_mp,
    double *fit_err_ptr
);

int homography_inverse
(
    const Matrix *H_mp,
    const Matrix *mp,
    Matrix       **res_mpp
);

int homography_transform
(
    const Matrix *p_mp,
    const Matrix *x_mp,
    Matrix       **y_mpp
);

int constrained_homography_1
(
    const Matrix *H_mp
);

int constrained_homography_2
(
    const Matrix *H_mp
);

int get_unity_homography
(
    Matrix **h_mpp
);

int normalize_homography
(
    Matrix *h_mp
);

int verify_homography
(
    const Matrix *H_mp,
    const Matrix *x_mp,
    const Matrix *y_mp,
    double       *err_ptr
);

int verify_homography_set
(
    const Matrix_vector *H_mvp,
    const Matrix_vector *x_mvp,
    const Matrix_vector *y_mvp,
    Vector              **err_vpp
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
