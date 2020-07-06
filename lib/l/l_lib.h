
/* $Id: l_lib.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef L_LIB_INCLUDED
#define L_LIB_INCLUDED

/* Start for c2man */


#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


#define BUFF_GET_PROGRAM_NAME(x)   get_program_name(x, sizeof(x))


int get_program_name(char* buff, size_t buff_size);
int set_program_name(char* program_name);
void reverse_four_bytes(void* input, void* output);

/* Lindsay - Sept 28, 1999*/
void reverse_two_bytes(void* input, void* output);
/* End Lindsay - Sept 28, 1999*/

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

