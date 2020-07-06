
/* $Id: m_mat_flip.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2013 by members of the Interdisciplinary Visual
|  Intelligence Lab.
|
|  Personal and educational use of this code is granted, provided that this
|  header is kept intact, and that the authorship is not misrepresented, that
|  its use is acknowledged in publications, and relevant papers are cited.
|
|  For other use contact the author (kobus AT sista DOT arizona DOT edu).
|
|  Please note that the code in this file has not necessarily been adequately
|  tested. Naturally, there is no guarantee of performance, support, or fitness
|  for any particular task. Nonetheless, I am interested in hearing about
|  problems that you encounter.
|
|  Authors:  Kobus Barnard, Andrew Predoehl
* =========================================================================== */

#ifndef M_MAT_FLIP_H_INCLUDED_IVILAB
#define M_MAT_FLIP_H_INCLUDED_IVILAB

/* Start for c2man */

#include <m/m_matrix.h>

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int ow_horizontal_flip_matrix(Matrix*);

int ow_vertical_flip_matrix(Matrix*);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif /* __cplusplus */

#endif /* M_MAT_FLIP_H_INCLUDED_IVILAB */
