
/* $Id: copy_image.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "i/i_incl.h" 

#define NUM_ROWS   2000
#define NUM_COLS   2000
#define BASE_NUM_TRIES   100

/*ARGSUSED*/
int main(int argc, char **argv)
{
    IVI_image* ip = NULL;
    IVI_image* ip2 = NULL;
    int i;
    long memcpy_cpu = 0;
    long std_cpy_cpu   = 0; 
    double diff; 
    int  num_tries = BASE_NUM_TRIES;
    int  test_factor = 1;


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

    EPETB(set_heap_options("heap-checking", "f")); 

    if (is_interactive())
    {
        EPETB(set_verbose_options("verbose", "1")); 
    }

    ivi_l_set("debug", "2");
    ivi_l_set("page", "off");

    EPETE(get_zero_image(&ip, NUM_ROWS, NUM_COLS));

    for (i = 0; i < num_tries; i++)
    {
        int r;
        int c;

        ivi_l_set("use-memcpy", "f");

        r = NUM_ROWS * ivi_rand();
        c = NUM_COLS * ivi_rand();

        ip->pixels[ r ][ c ].r = i;
        ip->pixels[ r ][ c ].g = i + 1;
        ip->pixels[ r ][ c ].b = i + 2;

        init_cpu_time();
        EPETE(ivi_copy_image(&ip2, ip));
        std_cpy_cpu += get_cpu_time(); 

        compute_rms_image_difference(ip, ip2, &diff);

        if (diff > DBL_MIN) 
        {
            set_error("Max diff (%.3e) is not zero.", diff);
            return EXIT_FAILURE;
        }

        ivi_l_set("use-memcpy", "t");

        r = NUM_ROWS * ivi_rand();
        c = NUM_COLS * ivi_rand();

        ip->pixels[ r ][ c ].r = i;
        ip->pixels[ r ][ c ].g = i + 1;
        ip->pixels[ r ][ c ].b = i + 2;


        init_cpu_time();
        EPETE(ivi_copy_image(&ip2, ip));
        memcpy_cpu += get_cpu_time(); 

        compute_rms_image_difference(ip, ip2, &diff);

        if (diff > DBL_MIN) 
        {
            p_stderr("Max diff (%.3e) is not zero.", diff);
            return EXIT_BUG;
        }
    }

    if (is_interactive())
    {
        dbi(std_cpy_cpu);
        dbi(memcpy_cpu);
    }

    ivi_free_image(ip); 
    ivi_free_image(ip2); 

    return EXIT_SUCCESS; 
}

