
/* $Id: ivi_glob.c 4723 2009-11-16 18:57:09Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    char input_str[ 100 ];
   
    ivi_set_debug_level(0); 
    ivi_init();

    while (TRUE)
    {
        if (is_interactive())
        {
            if (BUFF_TERM_GET_LINE("cmd> ", input_str) == EOF) break; 
        }
        else
        {
            if (BUFF_FGET_LINE(stdin, input_str) == EOF)  break;
        }

        EPE(ivi_system(input_str));
    }

    return EXIT_SUCCESS; 
} 


/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

