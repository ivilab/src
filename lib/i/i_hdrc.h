
/* $Id: i_hdrc.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Lindsay Martin and Kobus Barnard (authors).
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

#ifndef I_HDRC_INCLUDED
#define I_HDRC_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* ------------------------------------------------------------------------ */

#ifndef HDRC_NUM_ROWS
#    define HDRC_NUM_ROWS          480
#endif

#ifndef HDRC_NUM_COLS
#    define HDRC_NUM_COLS          640
#endif

/* HDRC .idt file size in bytes
 * 16-bits / 8-bits-per-byte = 2 bytes-per-pixel
 */
#ifndef HDRC_RAW_16_BIT_SIZE
#    define HDRC_RAW_16_BIT_SIZE   (2 * HDRC_NUM_ROWS * HDRC_NUM_COLS)

#endif

/* ------------------------------------------------------------------------ */

typedef enum Hdrc_camera_type
{
    HDRC_MONOCHROME,
    HDRC_COLOUR
}
Hdrc_camera_type;

typedef enum Hdrc_pixel_correction_method
{
    NONE,
    SET_PIXEL_TO_ZERO,
    FLAG_PIXEL_AS_INVALID,
    AVERAGE_NEIGHBOURS
}
Hdrc_pixel_correction_method;

typedef enum Hdrc_neighbour_type
{
    NEAREST_NEIGHBOURS, SAME_SENSOR_CHANNEL
}
Hdrc_neighbour_type;

typedef enum Hdrc_neighbour_avg_space
{
    LOG_SENSOR_SPACE, LINEAR_INTENSITY_SPACE
}
Hdrc_neighbour_avg_space;

typedef struct Hdrc_neighbour_info
{
    int         sensor;
    Pixel_info  coords;
    Pixel data;
}
Hdrc_neighbour_info;

typedef struct Hdrc_neighbour_vector
{
    int             length;
    Hdrc_neighbour_info* elements;
}
Hdrc_neighbour_vector;
/* ------------------------------------------------------------------------ */

int set_hdrc_options(const char*, const char*);
int set_hdrc_fixed_pixel_correction_file(const char* file_name);
int ow_correct_hdrc_fixed_pixels(IVI_image* ip);
int ow_hdrc_demosaic(IVI_image* ip);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

