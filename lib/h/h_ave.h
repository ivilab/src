
/* $Id: h_ave.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef H_AVE_INCLUDED
#define H_AVE_INCLUDED

/* Start for c2man */


#include "h/h_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


#define NO_HULL_WEIGHT_FN       ((double(*)(const Vector*))NULL)
#define NO_HULL_PROJECTION_FN   ((int(*)(Vector**, const Vector*))NULL)


int set_hull_ave_options         (const char* option, const char* value);

int approximate_hull_average
(
    const Hull* hp,          /* Address of input hull. */
    Vector**    average_vpp, /* Address of vector describing average. */
    double*     vol_ptr
);

int get_hull_CM_and_volume
(
    const Hull* hp,          /* Address of input hull. */
    Vector**    average_vpp, /* Address of vector for average. */
    double*     volume_ptr
);

int find_weighted_hull_average
(
    const Hull* hp,                           /* Address of input hull. */
    double      (*weight_fn) (const Vector*), /* Weight function. */
    Vector**    average_vpp                   /* Address of average vector.*/
);

int find_constrained_hull_average
(
    const Hull* hp,                                         /* Address of input hull. */
    const Hull* inverse_hp,                                 /* Address of inverse hull. */
    int         (*projection_fn) (Vector**, const Vector*),
    double      (*weight_fn) (const Vector*),
    Vector**    average_vpp,                                /* Address of average vector. */
    Vector**    max_prod_vpp,
    Hull**      result_hp_ptr
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

