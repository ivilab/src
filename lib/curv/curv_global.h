
/* $Id: curv_global.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef CURV_GLOBAL_INCLUDED
#define CURV_GLOBAL_INCLUDED

/* Start for c2man */


#include "l/l_gen.h"
#include "curv/curv_def.h"

/* End of includes for c2man */

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

#ifdef CURV_ALLOCATE_GLOBAL_STORAGE
#    define GLOBAL_DEF  EXPORT
#    define GLOBAL_INIT(x) x
#else
#    define GLOBAL_DEF  IMPORT
#    define GLOBAL_INIT(x)
#endif


GLOBAL_DEF const int curv_neighbors[ CURV_NUM_DIRECTIONS ][ 2 ]
#ifndef __C2MAN__
#ifdef CURV_ALLOCATE_GLOBAL_STORAGE
    =
        {
            {-1, 0}, /* up */
            {-1, 1}, /* up right */
            { 0, 1}, /* right */
            { 1, 1}, /* down right */
            { 1, 0}, /* down */
            { 1,-1}, /* down left */
            { 0,-1}, /* left */
            {-1,-1}  /* up left */
        }
#endif
#endif
;


#undef GLOBAL_DEF
#undef GLOBAL_INIT


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


