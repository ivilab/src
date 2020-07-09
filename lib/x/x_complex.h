
/* $Id: x_complex.h 25615 2020-07-09 19:12:05Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 2003-2008 by Kobus Barnard (author).
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

#ifndef X_COMPLEX_INCLUDED
#define X_COMPLEX_INCLUDED

/* Start for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#include "l/l_def.h"

/* End of includes for c2man */

/*
// We can't use the preferred name "Complex", since it has a conflict with many
// headers. Although low level headers should really, really, REALLY, not use
// such a name, what can one do?
*/

typedef struct IVI_complex
{
   double r;
   double i;
}
IVI_complex;

/* -------------------------------------------------------------------------- */

IVI_complex make_complex            (double r, double i);
IVI_complex conjugate_of_complex    (IVI_complex z);
double      magnitude_of_complex    (IVI_complex z);
double      magnitude_of_complex_2  (double r, double i);
double      angle_of_complex        (IVI_complex z);
double      angle_of_complex_2      (double r, double i);
IVI_complex add_complex             (IVI_complex a, IVI_complex b);
IVI_complex subtract_complex        (IVI_complex a, IVI_complex b);
IVI_complex multiply_complex        (IVI_complex a, IVI_complex b);
IVI_complex divide_complex          (IVI_complex a, IVI_complex b);
IVI_complex multiply_complex_by_real(IVI_complex z, double s);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


