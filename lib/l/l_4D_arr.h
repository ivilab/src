
/* $Id: l_4D_arr.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef L_4D_ARRAY_INCLUDED
#define L_4D_ARRAY_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "l/l_sys_mal.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


double**** allocate_4D_double_array
(
    int num_blocks,
    int num_planes,
    int num_rows,
    int num_cols
);

float**** allocate_4D_float_array
(
    int num_blocks,
    int num_planes,
    int num_rows,
    int num_cols
);

void***** allocate_4D_ptr_array
(
    int num_blocks,
    int num_planes,
    int num_rows,
    int num_cols
);

void free_4D_ptr_array(void***** array);
void free_4D_double_array(double**** array);
void free_4D_float_array(float**** array);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

