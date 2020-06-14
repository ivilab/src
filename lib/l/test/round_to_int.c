
/* $Id: round_to_int.c 25499 2020-06-14 13:26:04Z kobus $ */


#define BASE_NUM_TRIES  3000

#include "l/l_incl.h" 

int main(int argc, char **argv)
{
    double dx;
    float  fx;
    int    i, j;
    int    ri, kri, dri, dkri; 
    int    test_factor = 1; 
    IMPORT int ivi_debug_level;
    int result = EXIT_SUCCESS;
    int num_tries = BASE_NUM_TRIES; 


    ivi_init();

    if (argc > 1)
    {
        EPETE(ss1pi(argv[ 1 ], &test_factor)); 

        if (test_factor == 0) 
        {
            num_tries = 100;
        }
        else
        {
            num_tries *= test_factor;
        }
    }

    if (is_interactive())
    {
        ivi_debug_level = 2;
    }
    else 
    {
        ivi_debug_level = 0; 
    }

    for (i = -num_tries; i< num_tries; i++)
    {
        dbi(i); 

        dx = (double)i + 0.5;
        ri = rint(dx);
        kri = ivi_rint(dx);

        if (kri != ri)
        {
            set_error("Test 1: Problem rounding double %.3e (%d != %d).", 
                      dx, kri, ri);
            ivi_print_error();
            result = EXIT_BUG;
        }

        dx = (double)i - 0.5;
        ri = rint(dx);
        kri = ivi_rint(dx);

        if (kri != ri)
        {
            set_error("Test 2: Problem rounding double %.3e (%d != %d).", 
                      dx, kri, ri);
            ivi_print_error();
            result = EXIT_BUG;
        }

        for (j = 0; j < num_tries; j++)
        {
            dx = (double)i + ivi_rand();
            ri = rint(dx);
            kri = ivi_rint(dx);

            if (kri != ri)
            {
                set_error("Test 3: Problem rounding double %.3e (%d != %d).", 
                          dx, kri, ri);
                ivi_print_error();
                result = EXIT_BUG;
            }
        }

#ifdef __GNUC__

        /*
         * Not all systems have rintf(). GNU systems generally have it. Test
         * ivi_rintf() on those systems for now. 
        */

        fx = (float)i + 0.5f;
        ri = rintf(fx);
        kri = ivi_rintf(fx);
        dri = rint((double)fx);
        dkri = ivi_rint((double)fx); 

        if (kri != ri)
        {
            set_error("Test 4: Problem rounding float %.3e (%d != %d)    [ Double results are %d and %d ].", 
                      (double)fx, kri, ri, dkri, dri);
            ivi_print_error();
            result = EXIT_BUG;
        }

        fx = (float)i - 0.5f;
        ri = rintf(fx);
        kri = ivi_rintf(fx);
        dri = rint((double)fx);
        dkri = ivi_rint((double)fx); 

        if (kri != ri)
        {
            set_error("Test 5: Problem rounding float %.3e (%d != %d)    [ Double results are %d and %d ].", 
                      (double)fx, kri, ri, dkri, dri);
            ivi_print_error();
            result = EXIT_BUG;
        }

        for (j = 0; j < num_tries; j++)
        {
            fx = (float)i + (float)ivi_rand();
            ri = rintf(fx);
            kri = ivi_rintf(fx);
            dri = rint((double)fx);
            dkri = ivi_rint((double)fx); 

            if (kri != ri)
            {
                set_error("Test 6: Problem rounding float %.3e (%d != %d)    [ Double results are %d and %d ].", 
                      (double)fx, kri, ri, dkri, dri);
                ivi_print_error();
                result = EXIT_BUG;
            }
        }
#endif 
    }

    ivi_cleanup();
    
    return result;
}

