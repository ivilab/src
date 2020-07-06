
/* $Id: i_gamma.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_GAMMA_INCLUDED
#define I_GAMMA_INCLUDED

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


int set_gamma_options(const char* option, const char* value);

int ow_gamma_correct_image(IVI_image* in_ip, const Vector* gamma_vp);

int gamma_correct_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Vector*    gamma_vp
);

int invert_image_gamma
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    Vector*          gamma_vp
);

int ow_invert_image_gamma(IVI_image* in_ip, Vector* gamma_vp);

int ow_linearize_pcd(IVI_image* ip);

int ow_apply_pcd_output_lut(IVI_image* ip);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


