
/* $Id: i_draw.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_DRAW_INCLUDED
#define I_DRAW_INCLUDED

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


int image_draw_contour
(
    IVI_image* ip,
    Matrix*    mp,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_segment
(
    IVI_image* ip,
    Vector*    p1_vp,
    Vector*    p2_vp,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_segment_2
(
    IVI_image* ip,
    int        i_beg,
    int        j_beg,
    int        i_end,
    int        j_end,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_gradient
(
    IVI_image* ip,
    Vector*    p1_vp,
    Vector*    p2_vp,
    int        width,
    int        r1,
    int        g1,
    int        b1,
    int        r2,
    int        g2,
    int        b2
);

int image_draw_gradient_2
(
    IVI_image* ip,
    int        i_beg,
    int        j_beg,
    int        i_end,
    int        j_end,
    int        width,
    int        r1,
    int        g1,
    int        b1,
    int        r2,
    int        g2,
    int        b2
);

int image_draw_points(IVI_image* ip, Matrix* mp, int r, int g, int b);

int image_draw_pixels
(
    IVI_image*  ip,
    int         num_pixels,
    Pixel_info* pixels,
    int         r,
    int         g,
    int         b
);

int image_draw_point
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_point_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    float      f_r,
    float      f_g,
    float      f_b
);

int image_draw_add_to_point
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_add_to_point_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    float      f_r,
    float      f_g,
    float      f_b
);

int image_draw_blend_with_point
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_blend_with_point_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        width,
    float      f_r,
    float      f_g,
    float      f_b
);
int image_draw_image
(
    IVI_image*       canvas_ip,
    const IVI_image* ip,
    int              i,
    int              j,
    int              scale
);

int image_draw_rectangle
(
    IVI_image* ip,
    int        i,
    int        j,
    int        height,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_rectangle_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        height,
    int        width,
    float      f_r,
    float      f_g,
    float      f_b
);

int image_draw_box
(
    IVI_image* out_ip,
    int        i,
    int        j,
    int        half_size,
    int        width,
    int        r,
    int        g,
    int        b
);

int image_draw_circle
(
    IVI_image* ip,
    int        i,
    int        j,
    int        radius,
    int        line_width,
    int        r,
    int        g,
    int        b
);

int image_draw_circle_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        radius,
    int        line_width,
    float      f_r,
    float      f_g,
    float      f_b
);

int image_draw_disk
(
    IVI_image* ip,
    int        i,
    int        j,
    int        radius,
    int        r,
    int        g,
    int        b
);


int image_draw_disk_2
(
    IVI_image* ip,
    int        i,
    int        j,
    int        radius,
    float      r,
    float      g,
    float      b
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


