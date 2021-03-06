/* $Id: similarity.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_SIMILARITY_INCLUDED
#define SLIC_SIMILARITY_INCLUDED

/* Start for c2man */

#include "slic/basic.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int fit_similarity
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    Matrix       **a_mpp,
    double       *fit_err_ptr
);

int similarity_inverse
(
    const Matrix *a_mp,
    const Matrix *mp,
    Matrix       **res_mpp
);

int similarity_transform_single_point
(
    const Matrix *a_mp,
    const Vector *x_vp,
    Vector       **y_vpp
);

int similarity_transform
(
    const Matrix *a_mp,
    const Matrix *x_mp,
    Matrix       **y_mpp
);

int get_similarity_fitting_error
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *a_mp,
    double *fit_err_ptr
);
        
int get_similarity_distance
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *a_mp, 
    Vector       **dist_vpp
);

int is_similarity_degenerate
(
    const Matrix *x_mp
);

int constrained_similarity
(
    const Matrix *a_mp
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
