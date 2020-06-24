
/* $Id: round.c 25587 2020-06-24 02:28:42Z kobus $ */


/* =========================================================================== *
|                                                                              |
|  Copyright (c) 1994-1998, by Kobus Barnard (author).                         |
|                                                                              |
|  For use outside the SFU vision lab please contact the author(s).            |
|                                                                              |
* =========================================================================== */

#include "l/l_incl.h" 

/* -------------------------------------------------------------------------- */

int main(void)
{
    char line[ 1000 ];
    double d;


    while (BUFF_STDIN_GET_LINE("double> ", line) != EOF)
    {
        if (ss1d(line, &d) == ERROR)
        {
            ivi_print_error();
        }
        else
        {
            pso("%d == %.1f?\n", ivi_rint(d), rint(d));
        }
    }
    
    return EXIT_SUCCESS; 
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

