
/* $Id: seg_ncuts.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               ||
|        Kobus Barnard.                                                        |
|        Prasad Gabbur.                                                        |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */


#ifndef NCUT_MODULE_INCLUDED
#define NCUT_MODULE_INCLUDED

/* Start for c2man */


/*
// Exported interfaces go here.
*/

#include "m/m_incl.h"
#include "n/n_incl.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int ncut_dense_bipartition       
(
    Vector**      softpartition_vpp,
    const Matrix* weight_mp 
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif    /*   #include this file            */


