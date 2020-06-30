/* Test Program for remove_matrix_row
 * Author: Liam Wilson
 * Date: June 17, 2020  */

#include "m/m_incl.h"

int main(int argc, char* argv[])
{
    int i,j,k;

    Matrix* mp = NULL;
    Vector* vp = NULL;
    Matrix* cp = NULL;

    /* Kobus: Check arguments, otherwise this program can crash. */
    check_num_args(argc, 4, 4, NULL);

    /* Assigns variables to the argument inputs
     * Note that the last input is either 1 for storing in vp or 0 to not store
     * it (e.g. we pass (Vector**)NULL 
     * Also note that we assume all test cases/input to be well-formed. */

    /* Kobus: Switch atoi, atof, to ivi library scan fumctions. */
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int row_to_remove = atoi(argv[3]);
    int pass_null = atoi(argv[4]);
    
    /* creates a random matrix to store in mp with specified dimensions */
    /* We copy this matrix for later checking */

    EPETE(get_random_matrix(&mp, rows, cols)); 
    copy_matrix(&cp,mp);
    /* Removes the specified row and stores if we specified to store it */

    if (pass_null == 0) 
    {
        EPETE(remove_matrix_row((Vector**)NULL, mp, row_to_remove));
    }
    else if (pass_null == 1)
    {
        EPETE(remove_matrix_row(&vp, mp, row_to_remove));
    }


    /* Confirms that we've removed a row */
    int num_rows = mp->num_rows;

    if (num_rows > (rows - 1)) 
    {
        p_stderr("Error: row not removed!\n");
        return EXIT_BUG;
    } 

    else if (num_rows > (rows-1)) 
    {
        p_stderr("Error: more than one row removed!\n");
        return EXIT_BUG;
    }

    /* Now we check for the values; if we've removed the correct row, the
     * subsequent rows will all be shifted by -1 (e.g. old R4 values will   
     * be in position R3 if we removed R2) but the prior rows will have the
     * same values in the same rows (in that example R1 vals will still be R1
     *
     * We also (first) check the values of the vector that should store the
     * removed row. */
   
    if (pass_null == 1)
    {
        for (k=0; k<cols; k++)
        {
            if (vp->elements[ k ] != cp->elements[ row_to_remove ][ k ])
            {
                p_stderr("Error: the row at column %d was not copied correctly\n", k);
                return EXIT_BUG;
            }
        }
    }
       
    
    for (i=0; i<rows; i++)
    {
        if (i == row_to_remove)
        {
            /* We 'continue' to skip the loop for the deleted row to avoid
             * throwing an error by iterating too many times. */
            continue;
        }

        for (j=0; j<cols; j++) 
        {
            if (i > row_to_remove)
            {
                /* We've reached the point where the indices of our modified
                 * array are shifted so we adjust as such. */
                if (cp->elements[ i ][ j ] != mp->elements[ i - 1 ][ j ]) 
                {
                    p_stderr("Error: wrong value copied at {%d,%d}\n",j,i);
                    return EXIT_BUG;
                } 
            }
            else 
            {
                /* We don't need to change it prior to hitting this point,
                 * however. */
                if (cp->elements[ i ][ j ] != mp->elements[ i ][ j ]) 
                {
                    p_stderr("Error: wrong value copied at {%d, %d}\n",j,i);
                    return EXIT_BUG;
                }
            }
        }
    }

    free_matrix(cp);
    free_matrix(mp);
    free_vector(vp);
    return EXIT_SUCCESS; 
    
}


