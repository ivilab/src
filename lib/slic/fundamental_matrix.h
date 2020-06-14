/* $Id: fundamental_matrix.h 25499 2020-06-14 13:26:04Z kobus $
 */
#ifndef SLIC_FUNDAMENTAL_MATRIX_DEFINED_H_
#define SLIC_FUNDAMENTAL_MATRIX_DEFINED_H_

#include "slic/basic.h"
#include "slic/homography.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int fit_fundamental_matrix
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    Matrix       **F_mpp,
    double       *fit_err_ptr
);

int get_fundamental_matrix_distance
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *F_mp, 
    Vector       **dist_vpp
);

int get_fundamental_matrix_fitting_error
(
    const Matrix *x_mp,
    const Matrix *y_mp,
    const Matrix *F_mp,
    double *fit_err_ptr
);

int is_fundamental_matrix_degenerate
(
    const Matrix *x_mp
);

int test_fundamental_matrix();

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif
