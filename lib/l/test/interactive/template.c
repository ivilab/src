
/* $Id: template.c 25499 2020-06-14 13:26:04Z kobus $ */


/* =========================================================================== *
|                                                                              |
|  Copyright (c) 1994-1998, by Kobus Barnard (author).                         |
|                                                                              |
|  For use outside the SFU vision lab please contact the author(s).            |
|                                                                              |
* =========================================================================== */

#include "l/l_incl.h" 

/* -------------------------------------------------------------------------- */

  
/* -------------------------------------------------------------------------- */

int main(int argc, char **argv)
{


    set_atn_trap(confirm_exit_sig_fn, DONT_RESTART_AFTER_SIGNAL); 

    /*
    // Not ablsolutely necessary, but more efficient to do it in advance. Also,
    // if we do not do it in advance, we can get bogus memory leak reports, as
    // then the implicit fork will occur when memory has been allocated, and we
    // don't neccessarily arrange for the free by the child process. 
    */
    EPETE(create_system_command_process()); 


    EPETE(ivi_l_set("exchange-usr-and-net", "t")); 
    EPETE(ivi_l_set("disable-dir-open", "t"));
    EPETE(ivi_l_set("debug", "2")); 
    EPETE(ivi_l_set("verbose", "20"));





    ivi_cleanup(); /* Not needed on most platforms, but doing it twice is OK. */
    
    return EXIT_SUCCESS; 
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

