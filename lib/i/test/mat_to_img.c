/**
 * @file
 * @brief unit test for matrix_to_max_contrast_8bit_bw_image() function
 * @author Andrew Predoehl
 */
/*
 * $Id: mat_to_img.c 25499 2020-06-14 13:26:04Z kobus $
 */
#include <i/i_matrix.h>
#include <m/m_matrix.h>

int main()
{
    Matrix *mp = NULL;
    IVI_image *ip = NULL;

    EPETE(get_target_matrix(&mp, 2, 2));

    mp -> elements[0][0] = 0;
    mp -> elements[0][1] = 1;
    mp -> elements[1][0] = 2;
    mp -> elements[1][1] = 4;

    EPETE(matrix_to_max_contrast_8bit_bw_image(mp, &ip));

    ASSERT(2 == ip -> num_rows);
    ASSERT(2 == ip -> num_cols);
    ASSERT(0    == ip -> pixels[0][0].r);
    ASSERT(255  == ip -> pixels[1][1].r);

    free_matrix(mp);
    ivi_free_image(ip);

    if (is_interactive())
    {
        ivi_puts("Success!\n");
    }

    return 0;
}

