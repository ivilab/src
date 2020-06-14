
/* $Id: jv_lap.h 25499 2020-06-14 13:26:04Z kobus $ */

/************************************************************************
*
*  lap.h
   version 1.0 - 21 june 1996
   author  Roy Jonker, MagicLogic Optimization Inc.

   header file for LAP
*
**************************************************************************/

#ifndef JV_LAP_INCLUDED
#define JV_LAP_INCLUDED


#include "m/m_incl.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int jv_lap
(
    const Matrix* cost_mp,
    Int_vector**  row_assignment_vpp,
    double*       cost_ptr
);

int int_jv_lap
(
    const Int_matrix* cost_mp,
    Int_vector**      row_assignment_vpp,
    int*              cost_ptr
);

int dbl_jv_lap
(
    const Matrix* cost_mp,
    Int_vector**  row_assignment_vpp,
    double*       cost_ptr
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif



