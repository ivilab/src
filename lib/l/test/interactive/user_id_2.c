
/* $Id: user_id_2.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_incl.h" 


/*ARGSUSED*/
int main(int argc, char **argv)
{
    char user_id[ 100 ];

    extern int ivi_debug_level;

    ivi_debug_level = 2; 

    EPETE(get_user_id(user_id, sizeof(user_id)));

    ivi_fputs(stdout,"User id is: ");
    toggle_high_light(stdout); 
    ivi_fputs(stdout,user_id);
    toggle_high_light(stdout); 
    ivi_fputs(stdout,"\n");

    return EXIT_SUCCESS; 
}
