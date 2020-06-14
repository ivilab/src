
/* $Id: high.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_incl.h" 


/*ARGSUSED*/
main(int argc, char **argv)
{

    ivi_l_set("debug_level", "1"); 

    ivi_fputs(stdout,"not high-lighted.\n");
    ivi_fputs(stdout,"[1mhigh-lighted.[m\n");

    ivi_fputs(stdout,"not high-lighted.\n");
    toggle_high_light(stdout); 
    ivi_fputs(stdout,"high-lighted.\n");
    toggle_high_light(stdout); 

    ivi_fputs(stdout,"not high-lighted.\n");
    toggle_high_light(stdout); 
    ivi_fputs(stdout,"high-lighted.\n");
    toggle_high_light(stdout); 

    ivi_fputs(stdout,"not high-lighted.\n");
    toggle_high_light(stdout); 
    ivi_fputs(stdout,"high-lighted.\n");
    toggle_high_light(stdout); 

    return EXIT_SUCCESS; 
}
