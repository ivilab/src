
/* $Id: i2_draw_text.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I2_DRAW_TEXT_INCLUDED
#define I2_DRAW_TEXT_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int image_draw_text_top_left
(
    IVI_image*  ip,
    const char* text,
    int         i,
    int         j,
    const char* font_str
);

int image_draw_wrapped_text_top_left
(
    IVI_image*  ip,
    const char* text,
    int         i,
    int         j,
    int         width, 
    const char* font_str
);

int image_draw_text_center
(
    IVI_image*  ip,
    const char* text,
    int         i,
    int         j,
    const char* font_str
);

int get_wrapped_text_block_image
(
    IVI_image** ipp,
    const char* text_in,
    int         width,
    const char* font_str
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


