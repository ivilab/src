
/* $Id: get_matrix_from_matrix_vector.c 25597 2020-06-30 23:31:45Z kobus $ */


#include "m/m_incl.h" 

int main(void)
{
    Matrix_vector* mvp = NULL; 
    Matrix* mp = NULL;
    int i;
    int count; 


    EPETE(get_target_matrix_vector(&mvp, 7));

    for (count = 0; count < 10; count++)
    {
        dbp("#######################################################"); 
        for (i=0; i<7; i++)
        {
            if (ivi_rand() > 0.7)
            {
                EPETE(get_random_matrix(&(mvp->elements[ i ]), 
                                        (int)(1.1 + 5.0 * ivi_rand()), 5));
            }

            dbi(i); 
            db_mat(mvp->elements[ i ]);
        }

        dbp("--------------------------------------------"); 

        EPETE(get_matrix_from_matrix_vector(&mp, mvp));

        db_mat(mp); 
    }

    free_matrix(mp); 
    free_matrix_vector(mvp); 

    return EXIT_SUCCESS; 
}

