
/* $Id: axis_camera.h 25612 2020-07-06 22:17:23Z kobus $ */

#ifndef AXIS_CAMERA_H_INCLUDED
#define AXIS_CAMERA_H_INCLUDED

/* Start for c2man */


#include "camera/camera_bw_byte_image.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


typedef struct Axis_camera
{
    char *hostname;
    char *username;
    char *password;
    int camera_number;
} Axis_camera;

int set_zoom ( Axis_camera * camera, int zoom ); /* 0 to 1000 */
int set_pan ( Axis_camera * camera, int pan ); /* 0 to 1000 */
int set_tilt ( Axis_camera * camera, int tilt ); /* 0 to 1000 */

int get_image_from_axis_camera ( Camera_bw_byte_image ** image, Axis_camera * camera );

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
