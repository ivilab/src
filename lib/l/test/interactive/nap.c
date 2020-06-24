
/* $Id: nap.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int  main(void)
{

    /*CONSTCOND*/
    while ( TRUE )
    {
        int i;

        pso("Two seconds of dots.\n"); 
        for (i=0; i< 20; i++)
        {
            nap(100); 
            pso(".");
        }
        pso("\n\n");

        pso("Two seconds of silence.\n"); 
        sleep(2);
        
        pso("Two seconds of dots.\n"); 
        for (i=0; i< 10; i++)
        {
            nap(200); 
            pso(".");
        }
        pso("\n\n");

        pso("Two seconds of silence.\n"); 
        sleep(2);
        
        pso("Two seconds of dots.\n"); 
        for (i=0; i< 4; i++)
        {
            nap(500); 
            pso(".");
        }
        pso("\n\n");

        pso("Two seconds of silence.\n"); 
        sleep(2);
        
    }

    return EXIT_SUCCESS; 
}

