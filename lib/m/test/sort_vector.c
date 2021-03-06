
/* $Id: sort_vector.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "m/m_incl.h" 

#define BASE_NUM_TRIES   1000
#define BASE_LENGTH      10000

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;
    int  num_tries = BASE_NUM_TRIES;
    int  test_factor = 1;
    Vector* vp = NULL;
    int try_count, i; 


    ivi_init(); 

    if (argc > 1)
    {
        EPETE(ss1pi(argv[ 1 ], &test_factor)); 
    }

    if (test_factor == 0)
    {
        num_tries = 1;
    }
    else
    {
        num_tries *= test_factor;
    }

    for (try_count = 0; try_count < num_tries; try_count++)
    {
        int length = 1 + ivi_rint(1.0 + BASE_LENGTH * ivi_rand());

        EPETE(get_random_vector(&vp, length));
        EPETE(ascend_sort_vector(vp));

        for (i = 0; i < length - 1; i++)
        {
            if (vp->elements[ i ] > vp->elements[ i + 1 ])
            {
                p_stderr("Out of order elements.\n");
                status = EXIT_BUG;
            }
        }


        /*
         *  No descend_sort_vector() yet. 
         *

        EPETE(get_random_vector(&vp, length));
        EPETE(descend_sort_vector(vp));

        for (i = 0; i < length - 1; i++)
        {
            if (vp->elements[ i ] < vp->elements[ i + 1 ])
            {
                p_stderr("Out of order elements.\n");
                status = EXIT_BUG;
            }
        }

        */
    }

    free_vector(vp);

    return status; 
}

