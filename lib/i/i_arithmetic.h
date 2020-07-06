

/* $Id: i_arithmetic.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_ARITHMETIC_INCLUDED
#define I_ARITHMETIC_INCLUDED

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


int power_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    double           power
);

int ow_power_image(IVI_image* in_ip, double power);

int scale_image(IVI_image**, const IVI_image*, double);

int ow_scale_image(IVI_image*, double);

int scale_image_by_channel(IVI_image**, const IVI_image*, Vector*);

int ow_scale_image_by_channel(IVI_image*, Vector*);

int subtract_images
(
    IVI_image**      target_ipp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip
);

int ow_subtract_images(IVI_image* in1_ip, const IVI_image* in2_ip);

int multiply_images
(
    IVI_image**      out_ipp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip
);

int ow_multiply_images(IVI_image* in1_ip, const IVI_image* in2_ip);

int divide_images
(
    IVI_image**      out_ipp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip
);

int ow_divide_images(IVI_image* in1_ip, const IVI_image* in2_ip);

int ow_min_of_images(IVI_image* in1_ip, const IVI_image* in2_ip);

int add_images
(
    IVI_image**      out_ipp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip
);

int ow_add_images(IVI_image* in1_ip, const IVI_image* in2_ip);

int ow_subtract_vector_from_image(IVI_image* ip, const Vector* vp);
int ow_add_vector_to_image(IVI_image* ip, const Vector* vp);
int ow_min_thresh_image(IVI_image* ip, double min);
int ow_max_thresh_image(IVI_image* ip, double max);

int log_one_plus_image(IVI_image** out_ipp, const IVI_image* in_ip);

int ow_log_one_plus_image(IVI_image* in_ip);

int ow_log_brightness_image
(
    IVI_image* in_ip,
    double     (*brightness_fn) (double, double, double),
    double     power
);

int ow_exponantiate_image(IVI_image* in_ip);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


