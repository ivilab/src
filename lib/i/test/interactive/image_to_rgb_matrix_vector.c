
/* $Id: image_to_rgb_matrix_vector.c 25499 2020-06-14 13:26:04Z kobus $ */


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
    IVI_image* out_ip = NULL;
    static Matrix* mp_list[ 3 ];

    ivi_init();   /* Best to do this if using IVI library. */

    if (! is_interactive()) 
    {
        p_stderr("This test program needs to be adjusted for batch testing.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    
    create_image_display();
    
    check_num_args(argc, 1, 1,  "image_draw_point [ file_name ]");
    EPETE(ivi_read_image(&ip, argv[ 1 ]));

    EPETE(image_to_rgb_matrix_array(ip, mp_list));

    EPETE(rgb_matrix_array_to_image(mp_list, &out_ip));

    EPETE(ivi_display_image(out_ip, NULL));


    prompt_to_continue();

    ivi_free_image(ip);
    ivi_free_image(out_ip);

    free_matrix(mp_list[ 0 ]);
    free_matrix(mp_list[ 1 ]);
    free_matrix(mp_list[ 2 ]);

    return EXIT_SUCCESS; 
} 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

