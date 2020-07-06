
/* $Id: camera_bw_byte_image_io.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|        Kate Spriggs                                                          |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef I_CAMERA_BW_BYTE_IO_H_INCLUDED
#define I_CAMERA_BW_BYTE_IO_H_INCLUDED

/* Start for c2man */

  
#include "camera/camera_bw_byte_image.h"  
#include "i/i_bw_byte_io.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


  int write_camera_bw_byte_image(Camera_bw_byte_image *image, const char * file_name);
  int read_camera_bw_byte_image(Camera_bw_byte_image **image, const char * file_name);
 
  
#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

