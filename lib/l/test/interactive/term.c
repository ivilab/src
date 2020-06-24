
/* $Id: term.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

Queue_element *string_queue_head, *string_queue_tail; 


int main(void)
{
    char line[ 3000 ];
    int  res;


    term_set_raw_mode_with_no_echo();

    set_atn_trap(raw_mode_with_no_echo_sig_fn, DONT_RESTART_AFTER_SIGNAL); 
    ivi_signal(SIGTERM, reset_term_before_default_sig_fn);
    ivi_signal(SIGQUIT, reset_term_before_default_sig_fn);
    ivi_signal(SIGTTIN, reset_term_before_default_sig_fn);
    ivi_signal(SIGTTOU, reset_term_before_default_sig_fn);
    ivi_signal(SIGXCPU, reset_term_before_default_sig_fn);
    ivi_signal(SIGXFSZ, reset_term_before_default_sig_fn);

    while ((res=BUFF_STDIN_GET_LINE("abc>",line)) != EOF)
    {
        EPETE(res);

        dbs(line); 
        dbi(res); 
    }

    term_reset();

    return EXIT_SUCCESS; 
}

