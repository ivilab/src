
#include "l/l_incl.h"

void cat_dir_name_fn(int __attribute__((unused)) sig);

int main (void)
{
    char dir_name[ MAX_FILE_NAME_SIZE ];
    FILE* pid_fp = NULL;
    FILE* dir_fp = NULL;
    struct sigaction handler_vec; 
    sigset_t blank_mask;


    NPETE(pid_fp = kjb_fopen("relay_ties_directories.pid", "w"));
    EPETE(kjb_fprintf(pid_fp, "%u\n", MY_PID)); 
    EPETE(kjb_fclose(pid_fp)); 

    handler_vec.sa_flags = SA_RESTART; 
    EPETE(kjb_sigemptyset(&handler_vec.sa_mask));
    handler_vec.sa_handler = cat_dir_name_fn; 

    EPETE(kjb_sigvec(30, &handler_vec, NULL));

    EPETE(kjb_sigemptyset(&blank_mask));

    while (TRUE)
    {
        (void) sigsuspend(&blank_mask);
        dbw(); 
    }
}

/* void cat_dir_name_fn(int __attribute__((unused)) sig)*/
void cat_dir_name_fn(int sig)
{
    dbi(sig);
}



