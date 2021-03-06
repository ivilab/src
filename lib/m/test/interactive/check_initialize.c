
/* $Id: check_initialize.c 25597 2020-06-30 23:31:45Z kobus $ */


/*
// This program tests the checking of memory intialization in the case of
// vectors and matrices. However, this checking is currently disabled. 
// If it were enabled, then this program should cause a request for abort,
// because the memory is not initialized.
*/

#include "m/m_incl.h" 

int main(void)
{
    Vector* vp = NULL; 
    Matrix* mp = NULL; 
    int i; 
    IMPORT int ivi_debug_level; 

    ivi_debug_level = 100; 

    ERE(get_target_vector(&vp, 10));

    for (i=0; i<vp->length - 1; i++) 
    {
        vp->elements[ i ] = 0.0; 
    }
    
    ERE(get_target_matrix(&mp, 10, 10));

    for (i=0; i<mp->num_rows; i++) 
    {
        mp->elements[ i ][ i ] = 0.0; 
    }

    free_vector(vp); 
    free_matrix(mp); 

    return EXIT_SUCCESS; 
}

