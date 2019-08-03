
/* $Id: s_def.h 24326 2019-08-01 16:29:32Z ivilab $ */

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

#ifndef S_DEF_INCLUDED
#define S_DEF_INCLUDED 1


#include "l/l_incl.h"
#include "m/m_incl.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace kjb_c {
#endif
#endif


#define NUM_SPECTROMETER_INTERVALS          101
#define SPECTROMETER_OFFSET                 380.0
#define SPECTROMETER_STEP                     4.0


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

