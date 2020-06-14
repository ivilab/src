
/* $Id: malloc_overun.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h"

/*ARGSUSED*/
int main(int argc, char **argv)
{
    int    i;
    unsigned char* uchar_mem;
    int*   int_mem;


    ivi_l_set("debug_level", "1"); 

    for (i=0; i<12; i++)
    {
        dbi(i); 
        int_mem = (int*)IVI_CALLOC(10, sizeof(int));
        dbi(int_mem[ 9 ] ); 
        dbi(int_mem[ 10 ] ); 
        dbi(int_mem[ 11 ] ); 
        int_mem[ i ] = -999;
        ivi_free(int_mem);
    }

    for (i=0; i<13; i++)
    {
        dbi(i); 
        uchar_mem = BYTE_MALLOC(10);
        dbc(uchar_mem[ 9 ] ); 
        dbc(uchar_mem[ 10 ] ); 
        dbc(uchar_mem[ 11 ] ); 
        uchar_mem[ i ] = '0';
        ivi_free(uchar_mem);
    }

    return EXIT_SUCCESS; 
}



