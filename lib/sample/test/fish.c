/*
 * $Id: fish.c 25609 2020-07-03 19:17:54Z kobus $
 */

#include <l/l_sys_io.h>
#include <sample/sample_misc.h>

int main(void)
{
    int histo[10] = {0,0,0,0,0,0,0,0,0,0};
    int j;
    double lambda = 4;
    
    for (j = 0; j < 1000; ++j) 
    {
        int x_poisson = poisson_rand(lambda);
        if (x_poisson < 0) return EXIT_FAILURE;
        if (x_poisson > 9) continue; /* rare but possible */
        histo[ x_poisson ] += 1;
    }

    /* Poisson(lambda=4) has pmf with peaks at 3 and 4.
     * So the mass at 3 should exceed the mass at 2, 5.
     * Ditto for the mass at 4.
     *
     * To repeat:  THE CRITERION BELOW IS CUSTOMIZED FOR LAMBDA OF FOUR.
     */
    if (    histo[3] < histo[2]
        ||  histo[3] < histo[5]
        ||  histo[4] < histo[2]
        ||  histo[4] < histo[5]
       )
    {
        ivi_fprintf(stderr, "Bad Poisson histogram (for lambda=%f):", lambda);
        for (j = 0; j < 10; ++j)
        {
            ivi_fprintf(stderr, " %d", histo[j]);
        }
        ivi_fprintf(stderr, "\n");
        return EXIT_BUG;
    }

    return EXIT_SUCCESS;
}
