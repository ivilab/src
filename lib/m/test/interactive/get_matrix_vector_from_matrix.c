
/* $Id: get_matrix_vector_from_matrix.c 25597 2020-06-30 23:31:45Z kobus $ */


#include "m/m_incl.h" 

#define NUM_ROWS   10
#define NUM_COLS   5

int main(void)
{
    Matrix_vector* mvp = NULL; 
    Matrix* mp = NULL;
    int count; 


    EPETE(get_random_matrix(&mp, NUM_ROWS, NUM_COLS));

    db_mat(mp);

    EPETE(get_matrix_vector_from_matrix(&mvp, mp));

    for (count = 0; count < NUM_ROWS; count++)
    {
        dbp("#######################################################"); 
        db_mat(mvp->elements[ count ]);
    }

    dbp("#######################################################"); 

    free_matrix(mp); 
    free_matrix_vector(mvp); 

    return EXIT_SUCCESS; 
}

