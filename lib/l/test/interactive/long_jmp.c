
/* $Id: long_jmp.c 25587 2020-06-24 02:28:42Z kobus $ */



#include "l/l_incl.h" 
#include <setjmp.h> 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

int main(void)
{
    IMPORT jmp_buf timer_interupt_env;
    char           line[ 200 ];
    int            res;


    res = NO_ERROR; 

    while (res != EOF) 
    {
        set_time_alarm_long_jump(5);

        if (setjmp(timer_interupt_env)) 
        {
            p_stderr("Timed out\n\n");
        }
        else 
        {
            res = BUFF_STDIN_GET_LINE("well? ", line); 
            p_stderr("NOT Timed out\n\n");
        }

        unset_time_alarm(); 
    }

    return EXIT_SUCCESS; 
}

