
/* $Id: sort_indexed_vector.c 25597 2020-06-30 23:31:45Z kobus $ */


#include "m/m_incl.h" 

#define LENGTH 1000
#define LOOPS 10

int main(void)
{
    Indexed_vector* vp = NULL;
    int i;
    int count;


    for (count = 0; count < LOOPS; count++)
    {
        EPETE(get_random_indexed_vector(&vp, LENGTH));

        if (LENGTH <= 30)
        {
            for (i = 0; i < LENGTH; i++)
            {
                dbi(vp->elements[ i ].index); 
            }

            for (i = 0; i < LENGTH; i++)
            {
                dbe(vp->elements[ i ].element); 
            }
        }

        EPETE(ascend_sort_indexed_vector(vp));

        if (LENGTH <= 30)
        {
            for (i = 0; i < LENGTH; i++)
            {
                dbi(vp->elements[ i ].index); 
            }

            for (i = 0; i < LENGTH; i++)
            {
                dbe(vp->elements[ i ].element); 
            }
        }

        EPETE(descend_sort_indexed_vector(vp));

        if (LENGTH <= 30)
        {
            for (i = 0; i < LENGTH; i++)
            {
                dbi(vp->elements[ i ].index); 
            }

            for (i = 0; i < LENGTH; i++)
            {
                dbe(vp->elements[ i ].element); 
            }
        }
    }

    free_indexed_vector(vp);

    return EXIT_SUCCESS; 
}

