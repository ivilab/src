
/* $Id: i_colour.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef I_COLOUR_INCLUDED
#define I_COLOUR_INCLUDED


#include "l/l_def.h"
#include "m/m_matrix.h"
#include "i/i_type.h"
#include "i/i_float.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int ow_match_chromaticity(IVI_image* in_ip, const IVI_image* chrom_ip);

int ow_match_brightness
(
    IVI_image*       in_ip,
    const IVI_image* brightness_ip,
    double           (*brightness_fn) (double, double, double)
);

double sRGB_Y_brightness(double r, double g, double b);
double rgb_sum_brightness(double r, double g, double b);
double rgb_gm_brightness(double r, double g, double b);

int make_chromaticity_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    double           intensity
);

int ow_make_chromaticity_image(IVI_image* ip, double intensity);

int make_black_and_white_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip
);

int ow_make_black_and_white_image(IVI_image* ip);

int convert_image_rgb_to_lab
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Vector*    white_rgb_vp
);

int convert_image_rgb_to_xyz(IVI_image** out_ipp, const IVI_image* in_ip);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


