
/* $Id: var_arg.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "l/l_incl.h" 


static TRAP_FN_RETURN_TYPE test_atn_fn(int);

int main(void)
{
    char line[ 3000 ];
    int   res, a, b, c, d, e;
    const char* line_pos;


    set_atn_trap(test_atn_fn, RESTART_AFTER_SIGNAL); 

    EPETE(term_puts("Enter 5 integers on a line.\n"));

    while ((res = BUFF_STDIN_GET_LINE("? ", line)) != EOF)
    {
        EPETE(res);

        dbs(line); 
        dbi(res); 

        line_pos = line; 

        if (get_n_pos_int(5, &line_pos, &a,&b,&c,&d,&e) != ERROR)
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

static TRAP_FN_RETURN_TYPE test_atn_fn(int __attribute__((unused)) dummy_sig_arg)
{

    term_puts("Trapped.\n");

}

