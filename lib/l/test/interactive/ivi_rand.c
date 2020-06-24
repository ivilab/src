
/* $Id: ivi_rand.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    int i;


    for (i=0; i<5; i++) 
    {
        ivi_seed_rand(99999999, 27); 
        pso("R1 %f    ", ivi_rand());
        pso("R2 %f\n", ivi_rand_2());
    }

    pso("\n");

    for (i=0; i<5; i++) 
    {
        ivi_seed_rand_2(99999999); 
        pso("R1 %f    ", ivi_rand());
        pso("R2 %f\n", ivi_rand_2());
    }

    return EXIT_SUCCESS; 
}

