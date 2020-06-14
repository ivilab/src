
/* $Id: file_age.c 25508 2020-06-14 15:15:09Z kobus $ */


#include "l/l_incl.h" 

/*ARGSUSED*/
int main(int argc, char **argv)
{
    time_t file_age; 

    check_num_args(argc, 1, 1, (char*)NULL);

    EPETE(get_file_age(argv[ 1 ], &file_age)); 

    pso("%lu\n",(unsigned long)file_age); 

    return EXIT_SUCCESS; 
}

