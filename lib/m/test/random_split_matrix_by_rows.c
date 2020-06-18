
/* $Id: remove_matrix_row.c 4723 2009-11-16 18:57:09Z kobus $ 
 * 
 * Author: Shreeya Jain
 * Date: June 18, 2020
 * Purpose: Tests the random_split_matrix_by_rows subroutine on various inputs
 * non-interactively. 
 */


#include "m/m_incl.h" 

/*ARGSUSED*/
int main(int argc, char **argv)
{
    Matrix* source_mp 	= NULL;		/* source matrix */
    Matrix* t1_mp 		= NULL;		/* target 1 matrix */
    Matrix* t2_mp 		= NULL;		/* target 2 matrix */

	/*
	 * Number of rows and columns of the source matrix
	 */
    int num_rows 		= atoi(argv[1]);	
    int num_cols 		= atoi(argv[2]);
    double fraction 	= atof(argv[3]);
    
    int num_rows_t1;
    int num_rows_t2;
    int i, j;
    int i_t1, i_t2;
    int j_t1, j_t2;
    
    /*
     * Create a random matrix to store in source_mp with the specified dimensions  
     */ 
    EPETE(get_random_matrix(&source_mp, num_rows, num_cols));
    
    /*
     * User must enter a fraction, ie, between 0 and 1
     * NOTE: this conditional return is present in m_mat_basic.c as well
     * However, an early condition helps prevent unnecessary computation and memory usage
     */
    if ((fraction < 0.0) || (fraction > 1.0))
    {
    	SET_ARGUMENT_BUG();
        return EXIT_FAILURE;	/* report user error */
    }
    
    EPETE(random_split_matrix_by_rows(&t1_mp, &t2_mp, source_mp, fraction)); 
    
    /*
     * Number of rows in the target matrices 1 and 2
     */
    num_rows_t1 = MAX_OF(0, MIN_OF(num_rows, ivi_rint(fraction * num_rows)));
    num_rows_t2 = num_rows - num_rows_t1;
    
    i_t1 = 0;
    i_t2 = 0;
    for (i = 0, i_t1 = 0, i_t2 = 0; i < num_rows; i++)
    {
    	j_t1 = 0;
    	j_t2 = 0;
        for (j = 0; j < num_cols; j++) 
        {
        	/* if the element is present in t1 */
        	if (i_t1 < num_rows_t1 && source_mp->elements[ i ][ j ] == t1_mp->elements[ i_t1 ][ j ])
            	j_t1++;
            /* if the element is present in t2 */
            else if (i_t2 < num_rows_t2 && source_mp->elements[ i ][ j ] == t2_mp->elements[ i_t2 ][ j ])
            	j_t2++;
            else
            {
            	p_stderr("Error in source_mp at {%d, %d}\n", i, j);
            	return EXIT_BUG;
            }
        }
        /* if the row is present in t1 */
        if (j_t1 == num_cols)
        	i_t1++;
        /* if the row is present in t2 */
        else if (j_t2 == num_cols)
        	i_t2++;
        
        else
        {
           	p_stderr("Error in source_mp at {*, %d}\n", j);
            return EXIT_BUG;
        }       
    }
    /* program reaches here only if no error was encountered, thus test successful! */
        
    free_matrix(source_mp); 
    free_matrix(t1_mp); 
    free_matrix(t2_mp); 

    return EXIT_SUCCESS; 
}

