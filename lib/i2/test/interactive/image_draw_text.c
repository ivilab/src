
/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */


#include "i2/i2_incl.h"

/* -------------------------------------------------------------------------- */

int main(void)
{
    IVI_image* ip = NULL; 


    ivi_init();   /* Best to do this if using IVI library. */
  
    create_image_display(); 
    
    EPETE(get_initialized_image_2(&ip, 400, 400, 100, 50, 0));

    EPETE(image_draw_text_top_left(ip, "Test one -- times14", 50, 50, NULL));
    EPETE(image_draw_text_top_left(ip, "Test two -- courier9", 150, 150, "courier9"));
    EPETE(image_draw_wrapped_text_top_left(ip, "Test\nthree\nwrapping with\ntimesb18", 
                                           200, 200, 150, "timesb18"));

    EPETE(ivi_display_image(ip, NULL));

    prompt_to_continue();

    ivi_free_image(ip);

    ivi_cleanup(); /* Almost never needed, but doing it twice is OK. */

    return EXIT_SUCCESS;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

