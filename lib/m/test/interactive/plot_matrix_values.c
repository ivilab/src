
/* $Id: plot_matrix_values.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "m/m_incl.h" 
#include "p/p_incl.h" 

int main(int argc, char **argv)
{
    Matrix* mp = NULL;
    int   plot_id; 
    int i, j; 


    EPETE(ivi_l_set("verbose", "1000"));

    EPETE(get_random_matrix(&mp, 10, 10));


    /*
    for (i=0; i<mp->num_rows; i++)
    {
        for (j=0; j<mp->num_cols; j++)
        {
            mp->elements[ i ][ j ] = i*j; 
        }
    }
    */

    EPETE(plot_id = plot_open3());

    EPETE(plot_matrix_values_2(plot_id, mp, 0.0, 1.0, 0.0, 1.0)); 

    continue_query(); 

    free_matrix(mp); 

    return EXIT_SUCCESS; 
}

