
/* $Id: n_set.c 25612 2020-07-06 22:17:23Z kobus $ */

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

#include "n/n_gen.h"     /* Only safe as first include in a ".c" file. */

#include "n/n_set.h"

#include "n/n_fit.h"
#include "n/n_invert.h"
#include "n/n_set.h"
#include "n/n_svd.h"
#include "n/n_cholesky.h"


/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

/*
 * =============================================================================
 *                                  ivi_n_set
 *
 *
 *
 *
 * Index:
 *     set option handling 
 *
 * -----------------------------------------------------------------------------
 */
int ivi_n_set(const char* option, const char* value)
{
    int         temp_result;
    int         result = NOT_FOUND;
    int         (**set_fn_ptr) ( const char*, const char* );
    static int  (*set_fn[]) ( const char*, const char* ) =
                                    {
                                        set_svd_options,
                                        set_matrix_inversion_options,
                                        set_least_squares_options,
                                        set_cholesky_options,
                                        NULL
                                    };

    set_fn_ptr = set_fn;

    while (*set_fn_ptr != NULL)
    {
        ERE(temp_result = (**set_fn_ptr)(option, value));

        if (temp_result == NO_ERROR) result = NO_ERROR;

        set_fn_ptr++;
    }

    return result;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef __cplusplus
}
#endif

