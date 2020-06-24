
/* $Id: copy_int_matrix.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "m/m_incl.h" 

#define MAX_NUM_ROWS   1000
#define MAX_NUM_COLS   1000
#define BASE_NUM_TRIES 500

int main(int argc, char **argv)
{
    Matrix* source_mp = NULL;
    Int_matrix* mp = NULL;
    Int_matrix* mp2 = NULL;
    int i;
    long memcpy_cpu = 0;
    long std_cpy_cpu   = 0; 
    int  num_tries = BASE_NUM_TRIES;
    int  test_factor = 1;


    ivi_init(); 

    if (argc > 1)
    {
        EPETE(ss1pi(argv[ 1 ], &test_factor)); 
    }

    if (test_factor <= 0)
    {
        num_tries = 1;
    }
    else
    {
        num_tries *= test_factor;
    } 

    ivi_l_set("debug", "2");
    ivi_l_set("page", "off");

    for (i = 0; i < num_tries; i++)
    {
        int    r;
        int    c;
        int    diff;
        int    num_rows = 1 + MAX_NUM_ROWS * ivi_rand();
        int    num_cols = 1 + MAX_NUM_COLS * ivi_rand();

        ivi_l_set("use-memcpy", "f");

        EPETE(get_random_matrix(&source_mp, num_rows, num_cols));
        EPETE(ow_subtract_scalar_from_matrix(source_mp, 0.5));
        EPETE(ow_multiply_matrix_by_scalar(source_mp, 10000.0));
        EPETE(copy_matrix_to_int_matrix(&mp, source_mp));

        r = ivi_rint((double)num_rows * ivi_rand());
        r = MIN_OF(r, num_rows - 1);

        c = ivi_rint((double)num_cols * ivi_rand());
        c = MIN_OF(c , num_cols - 1);

        ASSERT_IS_LESS_INT(r, num_rows);
        ASSERT_IS_LESS_INT(c, num_cols);

        mp->elements[ r ][ c ] = i;

        init_cpu_time();
        EPETE(copy_int_matrix(&mp2, mp));
        std_cpy_cpu += get_cpu_time(); 

        diff = max_abs_int_matrix_difference(mp, mp2);

        if (diff != 0) 
        {
            p_stderr("Max diff (%d) is not zero (without memcpy).\n", diff);
            return EXIT_BUG;
        }

        ivi_l_set("use-memcpy", "t");

        r = ivi_rint((double)num_rows * ivi_rand());
        r = MIN_OF(r, num_rows - 1);

        c = ivi_rint((double)num_cols * ivi_rand());
        c = MIN_OF(c , num_cols - 1);

        ASSERT_IS_LESS_INT(r, num_rows);
        ASSERT_IS_LESS_INT(c, num_cols);

        mp->elements[ r ][ c ] = i;

        init_cpu_time();
        EPETE(copy_int_matrix(&mp2, mp));
        memcpy_cpu += get_cpu_time(); 

        diff = max_abs_int_matrix_difference(mp, mp2);

        if (diff != 0) 
        {
            p_stderr("Max diff (%d) is not zero (memcpy).\n", diff);
            return EXIT_BUG;
        }
    }

    if (is_interactive())
    {
        dbi(std_cpy_cpu);
        dbi(memcpy_cpu);
    }

    free_matrix(source_mp); 
    free_int_matrix(mp); 
    free_int_matrix(mp2); 

    return EXIT_SUCCESS; 
}

