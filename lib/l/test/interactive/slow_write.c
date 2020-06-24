
/* $Id: slow_write.c 25587 2020-06-24 02:28:42Z kobus $ */



#include "l/l_incl.h" 

int main(void)
{
    char str[ 100 ];
    char *str_pos;
    int  i; 


    BUFF_CPY(str, "THE INPUT\n"); 

    for (i = 0; i<5; i++)
    {
        str_pos = str;

        while (*str_pos)
        {
            nap(300); 
            putchar(*str_pos++);
            fflush(stdout); 
        }
    }

    pso("Slow_write finishing up.\n"); 

    return EXIT_SUCCESS; 
}

