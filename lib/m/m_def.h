
/* $Id: m_def.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef M_DEF_INCLUDED
#define M_DEF_INCLUDED

/* Start for c2man */

#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/*
 * Transition!
*/
#define create_transpose create_matrix_transpose
#define get_transpose    get_matrix_transpose


#define MIN_ABS_NORMALIZATION_VALUE  (1000.0 * DBL_MIN)


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

