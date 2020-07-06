
/* $Id: n_diagonalize.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef N_DIAGONALIZE_INCLUDED
#define N_DIAGONALIZE_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int diagonalize(const Matrix* mp, Matrix** E_mpp, Vector** D_vpp);

int diagonalize_2
(
    const Matrix* mp,
    Matrix**      E_re_mpp,
    Matrix**      E_im_mpp,
    Vector**      D_re_vpp,
    Vector**      D_im_vpp
);

int diagonalize_symmetric
(
    const Matrix* mp,
    Matrix**      E_mpp,
    Vector**      D_vpp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

