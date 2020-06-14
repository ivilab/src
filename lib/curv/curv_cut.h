
/* $Id: curv_cut.h 25499 2020-06-14 13:26:04Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               |
|        Kobus Barnard                                                         |
|        Amy Platt                                                             |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef CURV_CUT_INCLUDED
#define CURV_CUT_INCLUDED


#include "i/i_incl.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int cut_neuron
(
    IVI_image**  cut_ipp,
    const IVI_image*  ip
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

