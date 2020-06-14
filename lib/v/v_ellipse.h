
/* $Id: v_ellipse.h 25499 2020-06-14 13:26:04Z kobus $ */

/*
    Copyright (c) 1994-2008 by Kobus Barnard (author).

    Personal and educational use of this code is granted, provided
    that this header is kept intact, and that the authorship is not
    misrepresented. Commercial use is not permitted.
*/

#ifndef V_ELLIPSE_INCLUDED
#define V_ELLIPSE_INCLUDED


#include "v/v_gen.h"
#include "g2/g2_ellipse.h"
#include "t3/t3_segment.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

int set_image_ellipse_options(const char* option, const char* value);

int find_ellipses
(
    Segmentation_t3*  segmentation_ptr,
    Ellipse_list** ellipse_list_ptr_ptr
);

Ellipse* fit_ellipse
(
    Matrix* outside_boundary_mp,
    double  min_axis_product,
    double  max_axis_product,
    double  max_hull_fit_error,
    double  max_abs_radial_error,
    double  max_rel_radial_error
);

int image_draw_ellipse(IVI_image* out_ip, Ellipse* ellipse_ptr);

int image_draw_ellipse_2
(
    IVI_image* out_ip,
    Ellipse*   ellipse_ptr,
    int        r,
    int        g,
    int        b
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


