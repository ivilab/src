
/* $Id: dump_core.c 4723 2009-11-16 18:57:09Z kobus $ */


#include "l/l_incl.h" 


int main(int argc, char **argv)
{
    int pid_to_wait_on; 
    int waited_on_process_exit;
    int waited_on_process_signal;
    int ivi_waitpid2_res; 

    check_num_args(argc, 1, 1, (char*)NULL); 

    EPETE(ss1pi(argv[ 1 ], &pid_to_wait_on));
    dbi(pid_to_wait_on);

    EPETE(ivi_waitpid2_res = ivi_waitpid2(pid_to_wait_on, &waited_on_process_exit, &waited_on_process_signal));

    dbi(ivi_waitpid2_res);
        
    if (ivi_waitpid2_res == PROCESS_IS_DEAD)
    {
        EPETE(pso("Process %d does not exist.\n", pid_to_wait_on));
    }
    else 
    {
        dbi(waited_on_process_exit);
        dbi(waited_on_process_signal);
    }

    return EXIT_SUCCESS; 
}


