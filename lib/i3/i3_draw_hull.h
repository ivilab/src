
/* $Id: i3_draw_hull.h 25612 2020-07-06 22:17:23Z kobus $ */

/*
    Copyright (c) 1994-2008 by Kobus Barnard (author).

    Personal and educational use of this code is granted, provided
    that this header is kept intact, and that the authorship is not
    misrepresented. Commercial use is not permitted.
*/

#ifndef I3_DRAW_HULL_INCLUDED
#define I3_DRAW_HULL_INCLUDED

/* Start for c2man */


#include "m/m_incl.h"
#include "h/h_gen.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int image_draw_hull_interior
(
    IVI_image*  ip,
    const Hull* hp,
    int         r,
    int         g,
    int         b
);

int image_draw_hull_boundary
(
    IVI_image*  ip,
    const Hull* hp,
    int         r,
    int         g,
    int         b
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


