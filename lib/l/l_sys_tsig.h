
/* $Id: l_sys_tsig.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef L_SYS_TSIG_INCLUDED
#define L_SYS_TSIG_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "l/l_sys_sig.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int                 set_time_alarm          (long);
int                 set_ms_time_alarm       (long);
int                 set_time_alarm_long_jump(long);
int                 unset_time_alarm        (void);
TRAP_FN_RETURN_TYPE time_out_fn             (TRAP_FN_ARGS);
TRAP_FN_RETURN_TYPE time_out_long_jump_fn   (TRAP_FN_ARGS);
int                 nap                     (long);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

