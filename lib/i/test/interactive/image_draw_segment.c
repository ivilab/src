
/* $Id: image_draw_segment.c 25499 2020-06-14 13:26:04Z kobus $ */


/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003, by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               ||
|        Kobus Barnard.                                                        |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

/*
//  Most programs need at least the "m" library. 
*/

#include "i/i_incl.h" 

/* -------------------------------------------------------------------------- */

int main(int argc, char** argv)
{
    IVI_image* ip = NULL;

    ivi_init();   /* Best to do this if using IVI library. */

    if (! is_interactive()) 
    {
        p_stderr("This test program needs to be adjusted for batch testing.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    
    create_image_display(); 

    EPETE(get_initialized_image_2(&ip, 1000, 1000, 0, 0, 0));

    EPETE(image_draw_segment_2(ip, 500, 500, 500, 0,      4, 200, 0, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 500, 1000,   4, 200, 0, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 0,   500,    4, 200, 0, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 1000, 500, 4, 200, 0, 0));

    EPETE(image_draw_segment_2(ip, 500, 0,      500, 500, 0, 200, 200, 200));
    EPETE(image_draw_segment_2(ip, 500, 1000,   500, 500, 0, 200, 200, 200));
    EPETE(image_draw_segment_2(ip, 0,   500,    500, 500, 0, 200, 200, 200));
    EPETE(image_draw_segment_2(ip, 1000, 500, 500, 500, 0, 200, 200, 200));

    EPETE(image_draw_segment_2(ip, 500, 500, 0, 0, 2, 0, 200, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 0, 1000, 2, 0, 200, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 1000, 0, 2, 0, 200, 0));
    EPETE(image_draw_segment_2(ip, 500, 500, 1000, 1000, 2, 0, 200, 0));

    EPETE(image_draw_segment_2(ip, 0, 0, 500, 500, 0, 200, 0, 200));
    EPETE(image_draw_segment_2(ip, 0, 1000, 500, 500, 0, 200, 0, 200));
    EPETE(image_draw_segment_2(ip, 1000, 0, 500, 500, 0, 200, 0, 200));
    EPETE(image_draw_segment_2(ip, 1000, 1000, 500, 500, 0, 200, 0, 200));

    EPETE(image_draw_segment_2(ip, 500, 500, 1000, 250, 0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 1000, 750, 0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 0,    750, 0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 0,    250, 0, 0, 0, 200));

    EPETE(image_draw_segment_2(ip, 500, 500, 750, 1000, 0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 250, 1000, 0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 750, 0,    0, 0, 0, 200));
    EPETE(image_draw_segment_2(ip, 500, 500, 250, 0,    0, 0, 0, 200));





    EPETE(image_draw_gradient_2(ip, 200, 200, 500, 0,      4, 255, 255, 255, 200, 0, 0));
    EPETE(image_draw_gradient_2(ip, 200, 200, 500, 1000,   4, 255, 255, 255, 200, 0, 0));
    EPETE(image_draw_gradient_2(ip, 200, 200, 0,   500,    4, 255, 255, 255, 200, 0, 0));
    EPETE(image_draw_gradient_2(ip, 200, 200, 1000, 500, 4, 255, 255, 255, 200, 0, 0));

    EPETE(image_draw_gradient_2(ip, 500, 0,      200, 200, 0, 255, 255, 255, 200, 200, 200));
    EPETE(image_draw_gradient_2(ip, 500, 1000,   200, 200, 0, 255, 255, 255, 200, 200, 200));
    EPETE(image_draw_gradient_2(ip, 0,   500,    200, 200, 0, 255, 255, 255, 200, 200, 200));
    EPETE(image_draw_gradient_2(ip, 1000, 200, 200, 500, 0, 255, 255, 255, 200, 200, 200));

    EPETE(image_draw_gradient_2(ip, 200, 200, 0, 0, 2, 255, 255, 255, 0, 200, 0));
    EPETE(image_draw_gradient_2(ip, 200, 200, 0, 1000, 2, 255, 255, 255, 0, 200, 0));
    EPETE(image_draw_gradient_2(ip, 200, 200, 1000, 0, 2, 255, 255, 255, 0, 200, 0));

    EPETE(image_draw_gradient_2(ip, 200, 200, 1000, 1000, 2, 255, 255, 255, 0, 200, 0));
    EPETE(image_draw_gradient_2(ip, 0, 0, 200, 200, 0, 255, 255, 255, 200, 0, 200));
    EPETE(image_draw_gradient_2(ip, 0, 1000, 200, 200, 0, 255, 255, 255, 200, 0, 200));
    EPETE(image_draw_gradient_2(ip, 1000, 0, 200, 200, 0, 255, 255, 255, 200, 0, 200));
    EPETE(image_draw_gradient_2(ip, 1000, 1000, 200, 200, 0, 255, 255, 255, 200, 0, 200));

    EPETE(image_draw_gradient_2(ip, 200, 200, 1000, 250, 0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 1000, 750, 0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 0,    750, 0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 0,    250, 0, 255, 255, 255, 0, 0, 200));

    EPETE(image_draw_gradient_2(ip, 200, 200, 750, 1000, 0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 250, 1000, 0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 750, 0,    0, 255, 255, 255, 0, 0, 200));
    EPETE(image_draw_gradient_2(ip, 200, 200, 250, 0,    0, 255, 255, 255, 0, 0, 200));

    EPETE(ivi_display_image(ip, NULL));

    prompt_to_continue();

    ivi_free_image(ip);
    
    return EXIT_SUCCESS; 
} 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

