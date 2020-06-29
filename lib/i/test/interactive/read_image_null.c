
/* $Id: read_image_null.c 25592 2020-06-29 19:12:01Z kobus $ */


#include "i/i_incl.h"

/*
 * Program to verify a sensible error mesasure if we try to open a file with a
 * null name. 
*/

int main(void)
{
    IVI_image* ip = NULL;

    ivi_init();   /* Best to do this if using IVI library. */

    /*
     * This does not do anything in batch mode. 
    */
    if (! is_interactive()) 
    {
        return EXIT_SUCCESS;
    }
    

    EPETE(ivi_read_image_2(&ip, ""));


    ivi_cleanup(); /* Almost never needed, but doing it twice is OK. */

    return EXIT_SUCCESS; 
} 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

