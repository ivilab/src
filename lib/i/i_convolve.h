
/* $Id: i_convolve.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_CONVOLVE_INCLUDED
#define I_CONVOLVE_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "m/m_vector.h"
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


int ow_gauss_convolve_image(IVI_image* ip, double sigma);

int gauss_convolve_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    double           sigma
);

int convolve_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Matrix* mask_mp
);

int x_convolve_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Vector*    mask_vp
);

int y_convolve_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    const Vector*    mask_vp
);

int gauss_sample_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    int              scale,
    double           sigma
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


