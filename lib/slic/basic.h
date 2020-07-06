/* $Id: basic.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_INCLUDED_BASIC_H_
#define SLIC_INCLUDED_BASIC_H_

/* Start for c2man */

#include "m/m_incl.h"
#include "n/n_incl.h" 
#include "i/i_incl.h" 
#include "i/i_float.h" 
/* #include "goto_lib.h" */
#include "slic/array.h"

/* End of includes for c2man */

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
/* End for c2man */

#endif

#endif
