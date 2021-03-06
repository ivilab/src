
/* $Id: a4.c 25592 2020-06-29 19:12:01Z kobus $ */


/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003, by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               ||
|        Kobus Barnard.                                                        |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#include "i/i_incl.h" 
#include "i2/i2_collage.h" 

/* -------------------------------------------------------------------------- */

#define NUM_EXPERIMENTS 6 
#define NUM_SCALES      5


int main(void)
{
    int        num_rows, num_cols;
    Matrix*    mp        = NULL;
    Matrix*    out_mp    = NULL;
    Matrix*    x_out_mp  = NULL;
    Matrix*    y_out_mp  = NULL;
    Matrix*    in_mp     = NULL;
    Matrix*    smooth_mp     = NULL;
    IVI_image* in_ip = NULL;
    IVI_image* out_ip    = NULL;
    Matrix*    x_mask_mp = NULL;
    Matrix*    x_combined_mask_mp = NULL;
    Matrix*    y_mask_mp = NULL;
    Matrix*    y_combined_mask_mp = NULL;
    int        scale;
    int        i, j;
    char       title_str[ 1000 ];
    double     sigma;
    IVI_image* collage_ip = NULL;
    int        collage_image_num = 0;
    Matrix*    gauss_2D_mp = NULL;
    char*      collage_image_titles[ NUM_SCALES * NUM_EXPERIMENTS ];
    /* Static to force initialization to 0. */
    static IVI_image* collage_images[ NUM_SCALES * NUM_EXPERIMENTS ];  


    ivi_init();   /* Best to do this if using IVI library. */

    if (! is_interactive()) 
    {
        p_stderr("This test program needs to be adjusted for batch testing.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    
    
    create_image_display();  /* Best to do this before displaying images. */
    
    EPETE(ivi_read_image(&in_ip, "input.tiff"));

    num_rows = in_ip->num_rows; 
    num_cols = in_ip->num_cols; 

    EPETE(ow_make_black_and_white_image(in_ip));
    EPETE(image_to_rgb_matrices(in_ip, &in_mp, NULL, NULL));

    for (scale = 0; scale < NUM_SCALES; scale++)
    {
        if (scale > 0) 
        {
            sigma = pow(2.0, (double)(scale - 1));
            EPETE(get_2D_gaussian_mask(&gauss_2D_mp, 1 + 3 * ipower(2,scale), sigma));  
            EPETE(convolve_matrix(&smooth_mp, in_mp, gauss_2D_mp));
        }
        else 
        {
            sigma = 0;
            EPETE(copy_matrix(&smooth_mp, in_mp));
        }

        EPETE(rgb_matrices_to_image(smooth_mp, smooth_mp, smooth_mp, &out_ip));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "Input, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        collage_images[ collage_image_num ] = NULL;
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 


        EPETE(get_target_matrix(&x_mask_mp, 1, 2));

        x_mask_mp->elements[ 0 ][ 0 ] = 1.0;
        x_mask_mp->elements[ 0 ][ 1 ] = -1.0;

        EPETE(convolve_matrix(&x_out_mp, smooth_mp, x_mask_mp));

        EPETE(copy_matrix(&out_mp, x_out_mp));
        EPETE(ow_multiply_matrix_by_scalar(out_mp, 256.0 / max_matrix_element(out_mp)));  
        EPETE(ow_add_scalar_to_matrix(out_mp, 128.0));      
        EPETE(rgb_matrices_to_image(out_mp, out_mp, out_mp, &out_ip));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "X edge, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 


        if (scale > 0) 
        {
            EPETE(convolve_matrix(&x_combined_mask_mp, gauss_2D_mp, x_mask_mp)); 
        }
        else
        {
            EPETE(copy_matrix(&x_combined_mask_mp, x_mask_mp));
        }

        EPETE(convolve_matrix(&x_out_mp, in_mp, x_combined_mask_mp));

        EPETE(copy_matrix(&out_mp, x_out_mp));
        EPETE(ow_multiply_matrix_by_scalar(out_mp, 256.0 / max_matrix_element(out_mp)));  
        EPETE(ow_add_scalar_to_matrix(out_mp, 128.0));      
        EPETE(rgb_matrices_to_image(out_mp, out_mp, out_mp, &out_ip));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "Combined X edge, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 


        EPETE(get_target_matrix(&y_mask_mp, 2, 1));

        y_mask_mp->elements[ 0 ][ 0 ] = 1.0;
        y_mask_mp->elements[ 1 ][ 0 ] = -1.0;

        EPETE(convolve_matrix(&y_out_mp, smooth_mp, y_mask_mp));

        EPETE(copy_matrix(&out_mp, y_out_mp));
        EPETE(ow_multiply_matrix_by_scalar(out_mp, 256.0 / max_matrix_element(out_mp)));  
        EPETE(ow_add_scalar_to_matrix(out_mp, 128.0));      
        EPETE(rgb_matrices_to_image(out_mp, out_mp, out_mp, &out_ip));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "Y edge, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 


        if (scale > 0) 
        {
            EPETE(convolve_matrix(&y_combined_mask_mp, gauss_2D_mp, y_mask_mp)); 
        }
        else
        {
            EPETE(copy_matrix(&y_combined_mask_mp, y_mask_mp));
        }

        EPETE(convolve_matrix(&y_out_mp, in_mp, y_combined_mask_mp));

        EPETE(copy_matrix(&out_mp, y_out_mp));
        EPETE(ow_multiply_matrix_by_scalar(out_mp, 256.0 / max_matrix_element(out_mp)));  
        EPETE(ow_add_scalar_to_matrix(out_mp, 128.0));      
        EPETE(rgb_matrices_to_image(out_mp, out_mp, out_mp, &out_ip));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "Combined Y edge, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 


        for (i = 0; i < out_mp->num_rows; i++)
        {
            for (j = 0; j < out_mp->num_cols; j++)
            {
                double ey = y_out_mp->elements[ i ][ j ];
                double ex = x_out_mp->elements[ i ][ j ];

                out_mp->elements[ i ][ j ] = sqrt(ex*ex + ey*ey);
            }
        }

        EPETE(ow_multiply_matrix_by_scalar(out_mp, 256.0 / max_matrix_element(out_mp)));  

        for (i = 0; i < num_rows; i++)
        {
            for (j = 0; j < num_cols; j++)
            {
                if (out_mp->elements[ i ][ j ] > 75.0)
                {
                    out_mp->elements[ i ][ j ] = 255.0;
                }
                else
                {
                    out_mp->elements[ i ][ j ] = 0.0;
                }
            }
        }

        EPETE(rgb_matrices_to_image(out_mp, out_mp, out_mp, &out_ip));
        EPETE(ow_max_thresh_matrix(out_mp, 128.0));
        EPETE(ivi_sprintf(title_str, sizeof(title_str), "Grad mag, sigma = %.1f", sigma));
        NPETE(collage_image_titles[ collage_image_num ] = ivi_strdup(title_str));
        EPETE(ivi_copy_image(&(collage_images[ collage_image_num ]), out_ip));
        collage_image_num++; 
    }

    EPETE(make_image_collage_with_labels(&collage_ip, NUM_SCALES, 6, collage_images, collage_image_titles));
    ivi_display_image(collage_ip, "collage");
    prompt_to_continue();
    
    for (i = 0; i < collage_image_num; i++)
    {
        ivi_free_image(collage_images[ i ]);
        ivi_free(collage_image_titles[ i ]);
    }

    ivi_free_image(collage_ip);

    free_matrix(mp);
    free_matrix(smooth_mp);
    free_matrix(out_mp);
    free_matrix(in_mp);
    free_matrix(gauss_2D_mp);
    free_matrix(x_mask_mp);
    free_matrix(x_out_mp);
    free_matrix(y_mask_mp);
    free_matrix(y_out_mp);
    free_matrix(y_combined_mask_mp);
    free_matrix(x_combined_mask_mp);

    ivi_free_image(in_ip);
    ivi_free_image(out_ip);

    ivi_cleanup(); /* Almost never needed, but doing it twice is OK. */

    return EXIT_SUCCESS; 
} 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

