
/* $Id: t2_set.h 25499 2020-06-14 13:26:04Z kobus $ */

/*
   Copyright (c) 1994-2008 by Kobus Barnard (author).

   Personal and educational use of this code is granted, provided
   that this header is kept intact, and that the authorship is not
   misrepresented. Commercial use is not permitted.
*/

#ifndef T2_SET_INCLUDED
#define T2_SET_INCLUDED


#include "l/l_def.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int ivi_t2_set(const char* option, const char* value);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


