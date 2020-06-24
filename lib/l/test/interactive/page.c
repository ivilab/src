
/* $Id: page.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h"

int  main(void)
{
    IMPORT int volatile ivi_tty_cols;
    unsigned int i;
    int          j;
    char*        str;
    char         line[ 200 ];


    /* Force tty open so that ivi_tty_cols is valid */
    term_puts("\n"); 

    for (i=0; i< sizeof(line)-ROOM_FOR_NULL;  i++) 
    {
        line[i] = '0' + i % 10;
    }

    line[ sizeof(line)- ROOM_FOR_NULL ] = '\0';

    line[ ivi_tty_cols - 1] = '*';
    line[ ivi_tty_cols ] = '\n';
    line[ ivi_tty_cols  + 1] = '\0';


    for (i=0; i<150; i++)
    {
        pso(line);
    }


    line[ ivi_tty_cols - 2] = '*';
    line[ ivi_tty_cols - 1] = '\n';
    line[ ivi_tty_cols] = '\0';


    for (i=0; i<150; i++)
    {
        pso(line);
    }


    line[ ivi_tty_cols - 3] = '*';
    line[ ivi_tty_cols - 2] = '\n';
    line[ ivi_tty_cols - 1] = '\0';


    for (i=0; i<150; i++)
    {
        pso(line);
    }


    NPETE(str=STR_MALLOC(100));

    for (i=0; i<10; i++) {
        for (j=0; j<100; j++) {
            sprintf(str, "<------%2d------->", j);
            ivi_fprintf(stderr, str);
        }
    }


    for (i=0; i<3000; i++) {
        strcpy(str, "123456789ABCD");
        ivi_fprintf(stderr, str);
    }

    ivi_free(str); 

    return EXIT_SUCCESS;
} 

