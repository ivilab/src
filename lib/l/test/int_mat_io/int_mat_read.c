
/* $Id: int_mat_read.c 25587 2020-06-24 02:28:42Z kobus $ */


#include "i/i_incl.h" 

int main(int argc, char *argv[])
{
    Int_matrix* mp       = NULL;


    ivi_init();

    check_num_args(argc, 1, 1, NULL); 

    create_image_display(); 

    EPETE(read_int_matrix(&mp, argv[ 1 ]));

    if ((mp->num_rows < 100) && (mp->num_cols < 10))
    {
        EPETE(write_int_matrix(mp, (char*)NULL));
    }
    else 
    {
        IVI_image* ip = NULL;
        int i, j; 

        EPETE(get_zero_image(&ip, mp->num_rows, mp->num_cols));

        for (i = 0; i < mp->num_rows; i++)
        {
            for (j = 0; j < mp->num_cols; j++)
            {
                if (mp->elements[ i ][ j ] == 0)
                {
                    ip->pixels[ i ][ j ].r = 200.0;
                    ip->pixels[ i ][ j ].g = 0.0;
                    ip->pixels[ i ][ j ].b = 0.0;
                }
                else if (mp->elements[ i ][ j ] == 1)
                {
                    ip->pixels[ i ][ j ].r = 0.0;
                    ip->pixels[ i ][ j ].g = 200.0;
                    ip->pixels[ i ][ j ].b = 0.0;
                }
                else if (mp->elements[ i ][ j ] == 2)
                {
                    ip->pixels[ i ][ j ].r = 0.0;
                    ip->pixels[ i ][ j ].g = 0.0;
                    ip->pixels[ i ][ j ].b = 200.0;
                }
                else if (mp->elements[ i ][ j ] == 3)
                {
                    ip->pixels[ i ][ j ].r = 200.0;
                    ip->pixels[ i ][ j ].g = 0.0;
                    ip->pixels[ i ][ j ].b = 200.0;
                }
                else if (mp->elements[ i ][ j ] == 4)
                {
                    ip->pixels[ i ][ j ].r = 0.0;
                    ip->pixels[ i ][ j ].g = 200.0;
                    ip->pixels[ i ][ j ].b = 200.0;
                }
                else if (mp->elements[ i ][ j ] == 5)
                {
                    ip->pixels[ i ][ j ].r = 200.0;
                    ip->pixels[ i ][ j ].g = 200.0;
                    ip->pixels[ i ][ j ].b = 0.0;
                }
                else if (mp->elements[ i ][ j ] == 6)
                {
                    ip->pixels[ i ][ j ].r = 200.0;
                    ip->pixels[ i ][ j ].g = 200.0;
                    ip->pixels[ i ][ j ].b = 200.0;
                }
            }
        }

        EPETE(ivi_display_image(ip, NULL));

        prompt_to_continue();

        ivi_free_image(ip); 
    }

    free_int_matrix(mp); 

    return EXIT_SUCCESS; 
}

