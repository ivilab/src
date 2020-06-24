
/* $Id: is_point_in_segment.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "g/g_incl.h" 

#define BASE_NUM_TESTS  10000

int main(int argc, char* argv[])
{
    int     i;
    int     dim;
    Vector* test_vp = NULL;
    Vector* p1_vp   = NULL;
    Vector* p2_vp   = NULL;
    Vector* t1_vp   = NULL;
    Vector* t2_vp   = NULL;
    int     inside; 
    int     num_tests = BASE_NUM_TESTS;
    int     test_factor = 1;


    ivi_init(); 

    if (argc > 1)
    {
        EPETE(ss1pi(argv[ 1 ], &test_factor)); 
    }

    if (test_factor <= 0)
    {
        num_tests = 1;
    }
    else
    {
        num_tests *= test_factor;
    } 

    if (is_interactive())
    {
        ivi_set_verbose_level(2);
    }
    else 
    {
        ivi_set_verbose_level(0); 
    }

    for (dim = 2; dim<20; dim++)
    {
        for (i=0; i<num_tests; i++)
        {
            double a = 3.0 * ivi_rand();

            if (is_interactive()) 
            {
                pso("%dD Test %d.\n", dim, i+1);
            }

            if (i==0) a = 0.0;
            if (i==1) a = 1.0;

            EPETE(get_random_vector(&p1_vp, dim));
            EPETE(get_random_vector(&p2_vp, dim));

            EPETE(multiply_vector_by_scalar(&t1_vp, p1_vp, a));
            EPETE(multiply_vector_by_scalar(&t2_vp, p2_vp, 1.0 - a));
            EPETE(add_vectors(&test_vp, t1_vp, t2_vp));

            EPETE(inside = is_point_in_segment(p1_vp, p2_vp, test_vp));

            if ((inside && (a > 1.0)) || ( !inside && (a <= 1.0)))
            {
                p_stderr("Result is in error.\n");
                return EXIT_BUG;
            }

        }
    }

    free_vector(t1_vp);
    free_vector(t2_vp);
    free_vector(p1_vp);
    free_vector(p2_vp);
    free_vector(test_vp);
        
    ivi_cleanup();

    return EXIT_SUCCESS; 
}

