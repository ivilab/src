
/* $Id: m_global.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef M_GLOBAL_INCLUDED
#define M_GLOBAL_INCLUDED


#include "l/l_gen.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


#ifdef GLOBAL_DEF
#    undef GLOBAL_DEF
#endif

#ifdef GLOBAL_INIT
#    undef GLOBAL_INIT
#endif

#ifdef M_ALLOCATE_GLOBAL_STORAGE
#    define GLOBAL_DEF  EXPORT
#    define GLOBAL_INIT(x) x
#else
#    define GLOBAL_DEF  IMPORT
#    define GLOBAL_INIT(x)
#endif


/*
 * Private! This is only exposed as a global for performance.
*/

#ifdef PROGRAMMER_IS_kobus
#ifdef TEST
GLOBAL_DEF int ivi_respect_missing_values   GLOBAL_INIT( = FALSE);
#else
GLOBAL_DEF int ivi_respect_missing_values   GLOBAL_INIT( = FALSE);
#endif
#else
GLOBAL_DEF int ivi_respect_missing_values   GLOBAL_INIT( = FALSE);
#endif


#undef GLOBAL_DEF
#undef GLOBAL_INIT


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


