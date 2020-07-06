
/* $Id: t2_set.c 25612 2020-07-06 22:17:23Z kobus $ */

/*
   Copyright (c) 1994-2008 by Kobus Barnard (author).

   Personal and educational use of this code is granted, provided
   that this header is kept intact, and that the authorship is not
   misrepresented. Commercial use is not permitted.
*/


#include "l/l_gen.h"     /* Only safe as first include in a ".c" file. */
#include "t2/t2_segment.h"
#include "t2/t2_set.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */

/*
 * =============================================================================
 *                            ivi_t2_set
 *
 *
 *
 *
 * Index:
 *     set option handling 
 *
 * -----------------------------------------------------------------------------
 */

int ivi_t2_set(const char* option, const char* value)
{
    int         temp_result;
    int         result = NOT_FOUND;
    int         (**set_fn_ptr)(const char*, const char*);
    static int  (*set_fn[])(const char*, const char*) =
                                    {
                                        t2_set_segmentation_options,
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

