
/* $Id: query.c 25499 2020-06-14 13:26:04Z kobus $ */



#include "l/l_incl.h" 

/*ARGSUSED*/
int main(int argc, char **argv)
{
    char line[ 100 ]; 


    set_atn_trap(confirm_exit_sig_fn, DONT_RESTART_AFTER_SIGNAL);

    put_prompt("Now you see it ..... (enter a char)> ");
    (void) overwrite_term_get_char(); 
    term_puts("Now you don't.\n");

    overwrite_query("OW-Q: Now you see it ..... (enter a line)> ", line, 100);
    term_puts("Now you don't.\n");
    term_puts(line); 
    term_puts("\n"); 

    ivi_query("IVI-Q: Now you see it ..... (enter a line)> ", line, 100);
    term_puts("Now you don't.\n");
    term_puts(line); 
    term_puts("\n"); 

    yes_no_query("Y-N-Q: Now you see it ..... (enter yes/no)> ");
    term_puts("Now you don't.\n");

    term_puts("Now use continue_query.\n");
    continue_query(); 
    term_puts("Now you don't.\n");

    return EXIT_SUCCESS; 
}

