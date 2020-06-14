
/* $Id: ivi_sprintf.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_incl.h" 

int main(void)
{
    IMPORT int ivi_debug_level; 
    char buff[ 1000 ]; 


    /*
    ivi_debug_level = 1;
    */

    EPETE(ivi_sprintf(buff, sizeof(buff), "%%A"));
    dbs(buff);
    put_line(buff);
    pso("%%A\n");

    EPETE(ivi_sprintf(buff, sizeof(buff), "%ld", 85));
    dbs(buff);
    pso("%ld\n", 85);



    return EXIT_SUCCESS; 
}

