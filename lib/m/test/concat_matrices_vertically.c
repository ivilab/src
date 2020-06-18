
/* $Id: concat_matrices_vertically.c 25499 2020-06-14 13:26:04Z kobus $ 
 * 
 * Author: Shreeya Jain
 * Date: June 17, 2020
 * Purpose: Tests the concat_matrices_vertically subroutine on various inputs
 * non-interactively. 
 * NOTE: The function (in m_mat_vector.c) returns an argument error if the
 * number of columns are not equal (test case 03).
 * TODO: Improve the automation of this test
 */


#include "m/m_incl.h" 

/*
 * NUM_MATRICES denotes the number of matrices (including NULL) that are to be
 * concatenated vertically
 * This needs to be edited for each test case
 */
#define NUM_MATRICES 3

#define MAX_FILENAME_LEN 256

void set_file_path(char *matrix_file, const char *filename);

/* 
 * Driver code for the test subroutine
 */
int main(void)
{
	Matrix *mp = NULL, *concat = NULL;
	Matrix *matrix_list[ NUM_MATRICES ];
    char file[ MAX_FILENAME_LEN ];
    /*
     * file list stores all the input files in base_path
     * This needs to be edited for each test case
     */
    const char *file_list[ NUM_MATRICES ] = 
    {"a.txt",
     "b.txt",
     "c.txt"
    };
    int i;
    double diff;	
    
    for (i = 0; i < NUM_MATRICES; i++)
    {
        matrix_list[ i ] = NULL;
    }
    
    for (i = 0; i < NUM_MATRICES; i++)
    {
    	Matrix *temp = NULL;
    	set_file_path(file, file_list[ i ]);
    	EPETE(read_matrix(&temp, file));
    	/* 
    	 * If the file is empty, store NULL matrix
    	 */
    	if (temp->num_cols != 0)
    	{
    		EPETE(copy_matrix(&matrix_list[ i ], temp));
    	}
    	else 
    	{
    		EPETE(copy_matrix(&matrix_list[ i ], NULL));
    	}
        free_matrix(temp);
    }
    
    EPETE(concat_matrices_vertically(&mp, NUM_MATRICES, matrix_list));
	
	/*
	 * concat stores the expected matrix after concatenation
	 */
	set_file_path(file, "concat.txt");
    EPETE(read_matrix(&concat, file)); 
    
    /* 
     * diff stores the max absolute difference between the calculated matrix
     * mp and the expected matrix concat. 
     * If the max absolute difference is 0.0, this means the two matrices are
     * equal.
     */
    diff = max_abs_matrix_difference(mp, concat);
	if (diff != 0.0)
	{
		p_stderr("Test failed!\n");
        return EXIT_BUG;
    }
    
    /* Cleanup */
    free_matrix(mp);
    free_matrix(concat);
    for (i = 0; i < NUM_MATRICES; i++)
    {
        free_matrix(matrix_list[ i ]);
    }
    
    return EXIT_SUCCESS; 
}

/*
 * Function to set the path of the input file as a concatenation of the 
 * base path and the filename
 * base_path is 00, 01 or 02 (currently)
 * filename is [a-z].txt or concat.txt
 */
void set_file_path(char *matrix_file, const char *filename)
{
  char base_path[] = "test_input/concat_matrices_vertically/00/";
  strcpy(matrix_file, base_path);
  strcat(matrix_file, filename);
}
