
/* $Id: i4_map.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I4_MAP_INCLUDED
#define I4_MAP_INCLUDED

/* Start for c2man */


#include "i4/i4_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int change_basis_post_map_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    Matrix*          map_mp,
    Matrix*          change_basis_mp
);

int change_basis_post_map_projected_image
(
    IVI_image**      out_ipp,
    const IVI_image* in_ip,
    Matrix*          change_basis_mp,
    Matrix*          map_mp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


