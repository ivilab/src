
/* $Id: l_type.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef L_TYPE_INCLUDED
#define L_TYPE_INCLUDED

/* Start for c2man */


#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


#if 0 /* was ifdef OBSOLETE */

/* Lets find out what we really need: May 26, 2005. */

#ifdef LONG_IS_64_BITS
#    define LONG_DATA LONG64_DATA
#    define ULONG_DATA ULONG64_DATA
#else
#ifdef LONG_IS_32_BITS
#    define LONG_DATA LONG32_DATA
#    define ULONG_DATA ULONG32_DATA
#endif
#endif

#ifdef INT_IS_32_BITS
#    define INT_DATA INT32_DATA
#    define UINT_DATA UINT32_DATA
#else
#ifdef INT_IS_16_BITS
#    define INT_DATA INT16_DATA
#    define UINT_DATA UINT16_DATA
#endif
#endif

#ifdef SHORT_IS_16_BITS
#    define SHORT_DATA SHORT16_DATA
#    define USHORT_DATA USHORT16_DATA
#endif

#ifdef INT16_IS_INT
#    define ss1i16     ss1i
#    define ss1pi16    ss1pi
#else
#ifdef INT16_IS_SHORT
#    define ss1i16     ss1s
#    define ss1pi16    ss1ps
#endif
#endif


typedef enum Data_type 
{
   CHAR_DATA,
   UCHAR_DATA,
   INT8_DATA,
   UINT8_DATA,
   INT16_DATA,
   UINT16_DATA,
   INT32_DATA,
   UINT32_DATA,
   FLOAT_DATA,
   DOUBLE_DATA
}
Data_type;

#endif


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

