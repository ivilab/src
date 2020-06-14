
/* $Id: var_arg.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "l/l_incl.h" 


static TRAP_FN_RETURN_TYPE test_atn_fn(int);

/*ARGSUSED*/
int main(int argc, char **argv)
{
    char line[ 3000 ];
    int   res, a, b, c, d, e;
    char* line_pos;


    set_atn_trap(test_atn_fn, RESTART_AFTER_SIGNAL); 

    EPETE(term_puts("Enter 5 integers on a line.\n"));

    while ((res = BUFF_STDIN_GET_LINE("? ", line)) != EOF)
    {
        EPETE(res);

        dbs(line); 
        dbi(res); 

        line_pos = line; 

        if (get_n_pos_int(5, (const char**)&line_pos, &a,&b,&c,&d,&e) != ERROR)
        {
            pso("%d %d %d %d %d\n", a,b,c,d,e);
            pso("Pos min is %d.\n", pos_min(5, a,b,c,d,e));
        }
        else 
        {
            ivi_print_error();
        }
    }

    return EXIT_SUCCESS; 
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*ARGSUSED*/
static TRAP_FN_RETURN_TYPE test_atn_fn(int dummy_sig_arg)
{

    term_puts("Trapped.\n");

}

