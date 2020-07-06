
/* $Id: camera_bw_byte_image.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|        Kate Spriggs                                                          |
|        Justin Cappos                                                         |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef CAMERA_BW_BYTE_IMAGE_H_INCLUDED
#define CAMERA_BW_BYTE_IMAGE_H_INCLUDED

/* Start for c2man */


#include "i/i_gen.h"
#include "i/i_bw_byte.h"
#include <sys/time.h>

#ifdef MS_OS
#   ifndef _WINSOCKAPI_

*struct timeval 
{
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};

#    endif     
#else
#    include <sys/time.h>
#endif 

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


typedef struct Camera_bw_byte_image
{
    Bw_byte_image *data;
    /*data has:
    int          num_rows;
    int          num_cols;
    int          read_only;
    unsigned char** pixels;
    */
    /*time will hold gettimeofday() value*/
    struct timeval image_time;
    /*from which camera on THIS machine does this image belong to*/
    char camera_id[128];
}Camera_bw_byte_image;

typedef struct Camera_face
{
    int x;
    int y;
    int width;
    int height;
    float orientation;
    /*so we know from where this face came from*/
    Camera_bw_byte_image * image_src;
}Camera_face;

int get_target_camera_bw_byte_image(Camera_bw_byte_image **target_ipp, int num_rows, int num_cols);
void ivi_free_camera_bw_byte_image(Camera_bw_byte_image* ip);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
