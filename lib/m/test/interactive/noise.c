
/* $Id: noise.c 25597 2020-06-30 23:31:45Z kobus $ */


#include "sample/sample_incl.h" 

int main(void)
{
    int i; 

   
    for (i=0; i<100000; i++) 
    {
        pso("%f\n", (double)gauss_rand()); 
    }

    return EXIT_SUCCESS; 
}

