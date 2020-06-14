
/* $Id: curv_curvature.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef CURV_CURVATURE_INCLUDED
#define CURV_CURVATURE_INCLUDED


#include "curv/curv_gen.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


double image_curvature(const IVI_image *image, 
                       Matrix** curvature_mpp,
                       int num_output_images, 
                       IVI_image** out_ip_list,
                       const char* file);

double image_curvature_tangent_weight(const IVI_image *image, 
                                      Matrix** curvature_mpp,
                                      Matrix** curvature_tangent_weight_vector_mpp,
                                      int num_output_images, 
                                      IVI_image** out_ip_list,
                                      const char* file);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

