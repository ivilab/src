
/* $Id: check_same_matrix_dimensions.c 4723 2009-11-16 18:57:09Z kobus $ */
/*
 * Author: Aditya Banerjee
 * Purpose: Checks the check_same_matrix_dimensions subroutine on various
 * inputs non-interactively.
 */

/* Kobus 20/06/30. Adjusted code to compile without warnings. */

#include "m/m_incl.h" 

#define MAX_FILENAME_LEN 256

/*
 * This function sets the value of the first string argument equal to the
 * name of the full file path to the second string argument. The full file
 * path in this case means the necessary prefix is attached to find the
 * input file in test_input/mat_error/check_same_matrix_dimensions/
 */
static void set_file_path(char matrix_file[ MAX_FILENAME_LEN ], 
                          const char* filename)
{
  char base_path[] = "test_input/mat_error/check_same_matrix_dimensions/";

  /* Kobus:
   * Copying strings without considering sizes is dangerous. These are the
   * famous buffer oveflow security vulnerablitiesl One way is to use
   * ivi_strcpy. 

  strcpy(matrix_file, base_path);
  strcat(matrix_file, filename);
  */ 
  ivi_strncpy(matrix_file, base_path, MAX_FILENAME_LEN);
  ivi_strncat(matrix_file, filename, MAX_FILENAME_LEN);
}

/*
 * The directive function that runs the tests.
 */
int main()
{
    Matrix* mp1 = NULL;
    Matrix* mp2 = NULL;

    /* Kobus: You want an array of characters, not an array of pointers to
     * characters,

    char *matrix_file[MAX_FILENAME_LEN];
    */
    char matrix_file[MAX_FILENAME_LEN];
    

    /* Tests expected to pass */
    
    set_file_path(matrix_file, "3x3_1.txt");
    EPETE(read_matrix(&mp1, matrix_file));  /* A 3x3 matrix */
        
    set_file_path(matrix_file, "3x3_2.txt");/* A different 3x3 matrix */
    EPETE(read_matrix(&mp2, matrix_file));  
   
    /* Check for the exact same matrix */
    EPE(check_same_matrix_dimensions(mp1, mp1, NULL));
    EPE(check_same_matrix_dimensions(mp2, mp2, NULL));
   
    /* Check two different mxn matrices */
    EPE(check_same_matrix_dimensions(mp1, mp2, NULL));
    EPE(check_same_matrix_dimensions(mp2, mp1, NULL));

    set_file_path(matrix_file, "3x3_3.txt");/* Another 3x3 matrix */
    EPETE(read_matrix(&mp2, matrix_file));  

    /* Check two mxn matrices but one of them has double digit values */
    EPE(check_same_matrix_dimensions(mp1, mp2, NULL));
    EPE(check_same_matrix_dimensions(mp2, mp1, NULL));


   
    /* Tests we expect to fail*/
    
    set_file_path(matrix_file, "2x7.txt");/* A 2x7 matrix */
    EPETE(read_matrix(&mp1, matrix_file));  
    
    set_file_path(matrix_file, "5x3.txt");/* A 5x3 matrix */
    EPETE(read_matrix(&mp2, matrix_file));  
   

    /* Check mxn matrix against pxq */
    int retval = check_same_matrix_dimensions(mp1, mp2, NULL);
    int retval2 = check_same_matrix_dimensions(mp2, mp1, NULL);
    if (retval != ERROR || retval2 != ERROR){ 
      printf("The test did not fail as expected\n");
    }
    
    /* Cleanup */
    free_matrix(mp1);
    free_matrix(mp2);

    return EXIT_SUCCESS; 
}
