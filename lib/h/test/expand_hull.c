
/* $Id: expand_hull.c 25866 2020-10-19 15:15:55Z kobus $ */


#include "h/h_incl.h" 

int main(void)
{
    Hull* hp;
    Hull* new_hp; 
    Matrix* mp = NULL;
    int i, j, k; 

    /*
    if (! is_interactive()) 
    {
        p_stderr("This test program either needs to be adjusted for batch testing or moved.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    */

    ivi_seed_rand(0, 0);

    for (k=0; k<2; k++)
    {
        EPETE(get_target_matrix(&mp, 5 + k, 2 + k));

        for (i = 0; i<mp->num_rows; i++)
        {
            for (j=0; j<mp->num_cols; j++)
            {
                mp->elements[ i ][ j ] = (int)(100.0 * ivi_rand());
            }
        }

        if (is_interactive()) 
        {
            db_mat(mp); 
        }

        NPETE(hp = find_convex_hull(mp, DEFAULT_HULL_OPTIONS)); 

        dbm("IL4RT: Setting expand-hulls-exactly to TRUE"); 
        set_hull_options("expand-hulls-exactly","t"); 
        NPETE(new_hp = expand_hull(hp, .1, DEFAULT_HULL_OPTIONS));
        if (is_interactive()) 
        {
            db_mat(new_hp->vertex_mp); 
        }

        free_hull(new_hp); 
        dbm("IL4RT: Setting expand-hulls-exactly to FALSE"); 
        set_hull_options("expand-hulls-exactly","f"); 
        NPETE(new_hp = expand_hull(hp, .1, DEFAULT_HULL_OPTIONS));
        if (is_interactive()) 
        {
            db_mat(new_hp->vertex_mp);  
        }

        free_hull(new_hp); 
        free_hull(hp);
    }

    free_matrix(mp); 

    return EXIT_SUCCESS; 
}

