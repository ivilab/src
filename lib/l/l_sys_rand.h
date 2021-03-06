
/* $Id: l_sys_rand.h 11541 2012-01-25 23:49:51Z kobus $ */

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

#ifndef L_SYS_RANDOM_INCLUDED
#define L_SYS_RANDOM_INCLUDED

/* Start for c2man */


#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

int    set_random_options        (const char* option, const char* value);
double ivi_rand_st               (void); /* single threaded random function */
double ivi_rand                  (void); /* standard U(0,1)-samp interface */
void   ivi_seed_rand_with_tod    (void);

void   ivi_seed_rand
(
    ivi_uint32 first_seed_value,
    ivi_uint32 second_seed_value
);

void   ivi_seed_rand_with_3_short(unsigned short buff[ 3 ]);
int    get_rand_seed             (unsigned short buff[ 3 ]);
double ivi_rand_2_st             (void); /* experts-only rand function */
double ivi_rand_2                (void); /* internal U(0,1)-samp interface */
void   ivi_seed_rand_2_with_tod  (void);
void   ivi_seed_rand_2           (long seed);

/* multithreaded interface:  we can change the function that is called. */
int    ivi_set_rand_function     (double (*f)(void));
int    ivi_set_rand_2_function   (double (*f)(void));

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

