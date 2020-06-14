/* $Id: gpu_cudpp.h 25499 2020-06-14 13:26:04Z kobus $ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
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
   |  Author:  Kyle Simek
 * =========================================================================== }}}*/

// vim: tabstop=4 shiftwidth=4 foldmethod=marker
#ifndef IVI_CPP_CUDPP_H
#define IVI_CPP_CUDPP_H

#include <l_cpp/l_exception.h>

#ifdef IVI_HAVE_CUDPP
#include <cudpp.h>

// TODO Create a CUDPP error class and convert error codes into meaningful strings
#define CUDPP_ETX(a) \
{ \
    CUDPPResult err = a; \
    if(err) \
    { \
        IVI_THROW_3(Runtime_error, "CUDPP error: %d", (err)); \
    } \
}

#endif

#endif 
