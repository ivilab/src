
/* $Id: hungarian.h 25499 2020-06-14 13:26:04Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 2000-2008 by Kobus Barnard (author), and UCB. Currently, the
|  use of this code is retricted to the UA and UCB image and text projects.
|
* =========================================================================== */

#ifndef HUNGARIAN_INCLUDED
#define HUNGARIAN_INCLUDED


#include "m/m_incl.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int hungarian
(
    const Matrix* cost_mp,
    Int_vector**  row_assignment_vpp,
    double*       cost_ptr
);

int int_hungarian
(
    const Int_matrix* cost_mp,
    Int_vector**      row_assignment_vpp,
    int*              cost_ptr
);



#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


