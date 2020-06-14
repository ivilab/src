
/* $Id: raw_mat_io.c 25499 2020-06-14 13:26:04Z kobus $ */


/*
 * Writes a random matrix in raw form. 
*/
#include "l/l_incl.h" 

int main(argc, argv)
    int argc;
    char *argv[];
{
    Int_matrix* mp       = NULL;
    int     num_rows;
    int     num_cols;

    ivi_init();
    check_num_args(argc, 1, 2, NULL); 

    EPETE(read_int_matrix(&mp, argv[ 1 ]));
    EPETE(write_raw_int_matrix(mp, (argc > 2) ? argv[ 2 ] : (char*)NULL));

    free_int_matrix(mp); 

    return EXIT_SUCCESS; 
}

