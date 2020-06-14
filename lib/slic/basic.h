/* $Id: basic.h 25499 2020-06-14 13:26:04Z kobus $
 */
#ifndef SLIC_INCLUDED_BASIC_H_
#define SLIC_INCLUDED_BASIC_H_

#include "m/m_incl.h"
#include "n/n_incl.h" 
#include "i/i_incl.h" 
#include "i/i_float.h" 
/* #include "goto_lib.h" */
#include "slic/array.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#define HOMOGRAPHY 1
#define AFFINE     2
#define SIMILARITY 3 

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif
