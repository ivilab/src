
/* $Id: canon.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

int main(void)
{
    int  done;
    int  res;


    done = FALSE;

    while (! done) 
    {
        res = term_getc();

        dbi(res); 

        if (res == 'n') 
        {
            dbi(term_set_raw_mode_with_no_echo()); 
        }
        else if (res == 'e')
        {
            dbi(term_set_raw_mode_with_echo()); 
        }
        else if (res == 'c')
        {
            dbi(term_set_cooked_mode()); 
        }
        else if (res == 'r')
        {
            p_stderr("Calling term_reset.\n"); 
            term_reset(); 
        }
        else if (res == EOF) 
        {
            done = TRUE; 
        }
    }

    return EXIT_SUCCESS; 
}

