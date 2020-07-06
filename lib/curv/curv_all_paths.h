
/* $Id: curv_all_paths.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               |
|        Kobus Barnard                                                         |
|        Amy Platt                                                             |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#ifndef CURV_ALL_PATHS_INCLUDED
#define CURV_ALL_PATHS_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int get_all_paths
(
    int               max_num_paths,
    int               min_len_for_stopping_at_junctions,
    int               depth_first_depth,
    int               max_path_len,
    const Int_matrix* on_mp,
    const Int_matrix* term_mp,
    const Matrix*     curvature_mp,
    Int_matrix*       i_mp,
    Int_matrix*       j_mp,
    Matrix*           t_mp,
    Int_vector*       len_vp,
    int               i,
    int               j,
    int               dir
);

#ifdef TEST
int verify_paths
(
    const Int_vector* len_vp,
    const Int_matrix* i_mp,
    const Int_matrix* j_mp
);
#endif

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

