
/* $Id: stat.c 25597 2020-06-30 23:31:45Z kobus $ */


#include "m/m_incl.h" 

int main(void)
{
    Vector* first_vp    = NULL;
    Vector* second_vp   = NULL;
    int     i;
    double    correlation;


    for (i=0; i<100; i++) 
    {
        EPETE(get_random_vector(&first_vp, 10000)); 
        EPETE(get_random_vector(&second_vp, 10000)); 
        EPETE(get_correlation_coefficient(first_vp, second_vp, &correlation));

        dbf(correlation); 
    }

    free_vector(first_vp);
    free_vector(second_vp); 

    return EXIT_SUCCESS; 
}

