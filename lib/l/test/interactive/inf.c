
/* $Id: inf.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_incl.h" 

int main(int argc, char **argv)
{
    float test, zero;
    ivi_int32 i;



    test = 10.0; 

    for (i=1; i< 50; i++) {
        dbi(i);
        dbf(test); 

        test *= 10.0;
    }

    zero = 0.0; 
    test = 1.0 / zero; 

    pso("\nSetting test to 1.0/0.0\n"); 
    dbf(test);
    dbx(test); 

    if (test > 1.0e100) 
    {
        pso("Inf beats 1.0e100.\n");
    }

    if (test > 1.0e70) 
    {
        pso("Inf beats 1.0e70.\n");
    }

    zero = 0.0; 
    test = -1.0 / zero; 

    pso("\nSetting test to -1.0/0.0\n"); 
    dbf(test);
    dbx(test); 

    if (test < -1.0e100) 
    {
        pso("Inf beats 1.0e100.\n");
    }

    if (test < -1.0e70) 
    {
        pso("Inf beats 1.0e70.\n");
    }

}

