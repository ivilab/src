
/* $Id: fget_line.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 

TRAP_FN_RETURN_TYPE test_atn_fn(int);

int main(void)
{
    char line[ 30 ];
    int  res;


    ivi_l_set("debug_level", "1"); 

    set_atn_trap(test_atn_fn, DONT_RESTART_AFTER_SIGNAL); 

    set_no_blocking(fileno(stdin));

    while ((res=fget_line(stdin, line,30)) != EOF) 
    {
        EPETE(res);

        dbs(line); 
        dbi(res); 

        nap(3000);

        dbm("Nap Done.\n");
    }

    dbi(res);
    return EXIT_SUCCESS; 
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

TRAP_FN_RETURN_TYPE test_atn_fn(int __attribute__((unused)) dummy_sig_arg)
{

    term_puts("Trapped.\n");

}


