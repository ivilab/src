
/* $Id: term_no_block.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 

Queue_element *string_queue_head, *string_queue_tail; 

/*ARGSUSED*/
int main(int argc, char **argv)
{
    char line[ 30 ]; 
    int  res; 


    term_set_no_blocking();
    EPETE(term_set_raw_mode_with_no_echo());

    EPETE(set_atn_trap(raw_mode_with_no_echo_sig_fn, 
                       DONT_RESTART_AFTER_SIGNAL));

    EPETE(ivi_signal(SIGTSTP, raw_mode_with_no_echo_sig_fn));
    EPETE(ivi_signal(SIGTERM, reset_term_before_default_sig_fn));
    EPETE(ivi_signal(SIGQUIT, reset_term_before_default_sig_fn));
    EPETE(ivi_signal(SIGTTIN, reset_term_before_default_sig_fn));
    EPETE(ivi_signal(SIGTTOU, reset_term_before_default_sig_fn));
    EPETE(ivi_signal(SIGXCPU, reset_term_before_default_sig_fn));
    EPETE(ivi_signal(SIGXFSZ, reset_term_before_default_sig_fn));

    ivi_l_set("debug_level", "1"); 

    while ((res=term_get_line("abc>",line,30)) != EOF)
    {
        EPETE(res);

        if (res != WOULD_BLOCK)
        {
            dbs(line); 
            dbi(res); 
        }

        nap(500);

        res=term_get_line("xyz>",line,30);

        if (res != WOULD_BLOCK)
        {
            dbs(line); 
            dbi(res); 
        }

        nap(500);

    }

    term_reset();
    set_blocking(fileno(stdin));

    return EXIT_SUCCESS; 
}

