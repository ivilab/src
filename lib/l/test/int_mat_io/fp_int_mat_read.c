
/* $Id: fp_int_mat_read.c 25499 2020-06-14 13:26:04Z kobus $ */


#include "i/i_incl.h" 

int main(int argc, char *argv[ ])
{
    Int_matrix* mp       = NULL;
    int         read_res;
    FILE* fp; 


    create_image_display(); 

    NPETE(fp = ivi_fopen(argv[ 1 ], "r")); 

    while ((read_res = fp_read_int_matrix(&mp, fp)) != EOF)
    {
        if (read_res == ERROR)
        {
            ivi_print_error();
            continue;
        }
 
        if ((mp->num_rows < 100) && (mp->num_cols < 20))
        {
            EPETE(write_int_matrix(mp, (char*)NULL));
            pso("\n\n"); 
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
    }

    ivi_fclose(fp); 
    free_int_matrix(mp); 

    return EXIT_SUCCESS; 
}

