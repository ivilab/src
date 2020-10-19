
/* $Id: distance_to_2D_hull.c 25866 2020-10-19 15:15:55Z kobus $ */


#include "h/h_incl.h" 

/*
// Remember: With bad luck, the approximate hulls can have a point arbitrarily
// far away from the correct hull, by having a very small angle at one of the
// vertices.
*/
#define STEP 0.01
#define BASE_NUM_TRIES   1000

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;
    int     i;
    double  x, y;
    Hull*   hp;
    Vector* grid_vp      = NULL;
    Vector* test_vp      = NULL;
    Matrix* point_mp     = NULL;
    int     num_points;
    double    d, distance;
    double    new_distance;
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

    EPETB(set_hull_options("hir", "100")); 
    EPETB(set_heap_options("heap-checking", "f")); 

    if (is_interactive())
    {
        EPETB(set_verbose_options("verbose", "2")); 
        EPETB(set_debug_options("debug-level", "2")); 
    }
    else 
    {
        EPETB(set_verbose_options("verbose", "0")); 
        EPETB(set_debug_options("debug-level", "0")); 

        /* We will get error messages, but the wrapper code to qhull does things
         * like perturbing the input and trying again. So for automatic testing, we
         * do no want to hear about it. 
        */
        EPETB(set_qhull_options("qhull-error-file", "/dev/null")); 
    }

    EPETE(get_target_vector(&grid_vp, 2)); 

    for (i=0; i<num_tries; i++)
    {
        if (is_interactive())
        {
            pso("Test %d.\n", i+1);
        }

        num_points = 5 + 10000.0 * ivi_rand();
        EPETE(get_random_matrix(&point_mp, num_points, 2));
        NPETE(hp = find_convex_hull(point_mp, DEFAULT_HULL_OPTIONS));
        EPETE(get_random_vector(&test_vp, 2));

        if (is_point_inside_hull(hp, test_vp))
        {
            get_interior_distance_to_hull(hp, test_vp, &distance);
            distance = -distance;
        }
        else 
        {
            distance = DBL_MAX;

            for (x = 0.0; x <= 1.0; x += STEP)
            {
                for (y = 0.0; y <= 1.0; y += STEP)
                {
                    int inside_hull;

                    grid_vp->elements[ 0 ] = x;
                    grid_vp->elements[ 1 ] = y;

                    EPETE(inside_hull = is_point_inside_hull(hp, grid_vp));

                    if (inside_hull)
                    {
                        d = vector_distance(grid_vp, test_vp);

                        if (d < distance) distance = d;
                    }
                }
            }

        }
        EPETB(get_distance_to_hull(hp, test_vp, &new_distance));

        if (ABS_OF(distance - new_distance) > 2.0 * STEP) 
        {
            dbf(distance);
            dbf(new_distance);
            status = EXIT_BUG;
        }

        free_hull(hp);

        ivi_flush();
    }

    free_vector(grid_vp);
    free_vector(test_vp);
    free_matrix(point_mp); 
        
    return status; 
}

