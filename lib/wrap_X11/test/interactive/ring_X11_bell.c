
/* $Id: ring_X11_bell.c 25609 2020-07-03 19:17:54Z kobus $ */


#include "l/l_incl.h" 
#include "wrap_X11/wrap_X11.h" 

int main(void)
{
    int i;


    for (i=0; i<5; i++) 
    {
        EPETE(ring_X11_bell());
        sleep(1);
    }

    return EXIT_SUCCESS; 
}

