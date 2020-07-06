
/* $Id: i_set.c 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author). (Lindsay Martin
|  contributed the interface to his hdrc code).
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

#include "i/i_gen.h"     /* Only safe as first include in a ".c" file. */

#include "i/i_ave.h"
#include "i/i_collage.h"
#include "i/i_demosaic.h"
#include "i/i_display.h"
#include "i/i_float_io.h"
#include "i/i_gamma.h"
#include "i/i_hdrc.h"      /* Lindsay - Nov 16, 1999 */
#include "i/i_html.h"
#include "i/i_offset.h"
#include "i/i_set.h"
#include "i/i_valid.h"


/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*
 * =============================================================================
 *                                  ivi_i_set
 *
 *
 *
 *
 * Index:
 *     set option handling 
 *
 * -----------------------------------------------------------------------------
 */

int ivi_i_set(const char* option, const char* value)
{
    int         temp_result;
    int         result = NOT_FOUND;
    int         (**set_fn_ptr) ( const char*, const char* );

    /* Lindsay - Nov 16, 1999
     * Added set_hdrc_options to following list.
     */
    /*
    // Note: Some option functions are not called here, as they force the load
    // of extra code and/or libraries. These include:
    //     set_video_options
    */
    static int  (*set_fn[]) ( const char*, const char* ) =
                                    {
                                        set_collage_options,
                                        set_demosaic_options,
                                        set_display_options,
                                        set_image_input_options,
                                        set_image_output_options,
                                        set_image_validity_options,
                                        set_gamma_options,
                                        set_hdrc_options,
                                        set_image_html_options,
                                        set_image_average_options,
                                        set_offset_removal_options,
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

