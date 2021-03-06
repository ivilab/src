
/* $Id: i_transform.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_TRANSFORM_INCLUDED
#define I_TRANSFORM_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "m/m_matrix.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int ow_invert_image(IVI_image* ip);
int ow_invert_gamma_image(IVI_image* ip);
int rotate_image_left(IVI_image** target_ipp, const IVI_image* ip);
int rotate_image_right(IVI_image** target_ipp, const IVI_image* ip);
int ow_horizontal_flip_image(IVI_image* ip);
int ow_vertical_flip_image(IVI_image* ip);

int scale_image_size
(
    IVI_image**      target_ipp,
    const IVI_image* ip,
    double           scale_factor
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


