
/* $Id: int_mat_io.c 25587 2020-06-24 02:28:42Z kobus $ */


/*
 * Writes a random matrix in raw form. 
*/
#include "l/l_incl.h" 

int main(argc, argv)
    int argc;
    char *argv[];
{
    Int_matrix* mp       = NULL;

    ivi_init();
    check_num_args(argc, 1, 2, NULL); 

    EPETE(read_int_matrix(&mp, argv[ 1 ]));
    EPETE(write_int_matrix(mp, (argc > 2) ? argv[ 2 ] : (char*)NULL));

    free_int_matrix(mp); 

    return EXIT_SUCCESS; 
}

