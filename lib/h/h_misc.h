
/* $Id: h_misc.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef H_MISC_INCLUDED
#define H_MISC_INCLUDED

/* Start for c2man */


#include "h/h_hull.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_hull_misc_options(const char* option, const char* value);

int get_interior_distance_to_hull
(
    const Hull*   hp,
    const Vector* test_vp,
    double*       distance_ptr
);

int get_distance_to_hull
(
    const Hull*   hp,
    const Vector* test_vp,
    double*       distance_ptr
);

Hull* expand_hull
(
    const Hull*   hp,
    double        relative_expansion,
    unsigned long options
);

int get_approximate_error_hull_data
(
    Matrix**      error_mpp,
    const Matrix* mp,
    int           block_size,
    const Hull*   hp
);

Hull* trim_2D_hull(const Hull* hp, double tolerance);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

