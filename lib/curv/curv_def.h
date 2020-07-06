
/* $Id: curv_def.h 25612 2020-07-06 22:17:23Z kobus $ */

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


#ifndef CURV_DEF_INCLUDED
#define CURV_DEF_INCLUDED

/* Start for c2man */

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


#define NEW_IMAGE_WAY

#define CURV_NUM_DIRECTIONS 8


#define IS_NEURON_PIXEL(ip,i,j) \
    ((i>= 0) && (i < ip->num_rows) && (j >= 0) && (j < ip->num_cols) && (ip->pixels[ i ][ j ].r > 128.0))

#define PIXEL_IS_ON(mp,i,j)  \
    ((i>= 0) && (i < mp->num_rows) && (j >= 0) && (j < mp->num_cols) && (mp->elements[ i ][ j ]))


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif    /*   #include this file            */

