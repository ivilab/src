
/* $Id: curv_type.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               ||
|        Kobus Barnard.                                                        |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */


/*
// Put defines here which are comon to more than one library module. Otherwise
// put it in the specific .h if it is exported, or the .c file if it is not.
*/

#ifndef CURV_TYPE_INCLUDED
#define CURV_TYPE_INCLUDED

/* Start for c2man */

#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


typedef struct ALP_Point
{
  /* the indices of this point */
  int i; 
  int j;
  /* the t-value associated with it, for solving parameterized representation */
  float t;
} ALP_Point;


typedef enum Curve_res
{
    CURVE_NO_ERROR = NO_ERROR,
    CURVE_ERROR    = ERROR,
    WRONG_NUMBER_OF_NEIGHBORS = ERROR - 1,
    NOT_LONG_ENOUGH = ERROR - 2,
    DEGENERATE_SYSTEM = ERROR - 3
}
Curve_res;


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif    /*   #include this file            */

