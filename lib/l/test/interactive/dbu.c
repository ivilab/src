
/* $Id: dbu.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 


int main(void)
{
    unsigned long  ui = 1;
    int i;

    for (i = 0; i < 40; i++)
    {
        dbi(ui);
        dbj(ui);
        dbu(ui);

        ui *= (unsigned int)2;
    }



    return EXIT_SUCCESS; 
}


