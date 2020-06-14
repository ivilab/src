
/* $Id: camera_bw_byte_image_io.c 25499 2020-06-14 13:26:04Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) by members of University of Arizona Computer Vision Group     |
|  (the authors) including                                                     |
|        Kobus Barnard.                                                        |
|        Kate Spriggs                                                          |
|        Justin Cappos                                                         |
|                                                                              |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */

#include "i/i_float.h"
#include "camera/camera_bw_byte_image_io.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


int write_camera_bw_byte_image(Camera_bw_byte_image *image, const char * file_name)
{
    int i, j;
    IVI_image *pImage = NULL;

    pImage = ivi_create_image(image->data->num_rows, image->data->num_cols);
    if (!pImage)
    {
        printf("Failed to create a IVI image \n");
        return -1;
    }
    for(i= 0; i < image->data->num_rows; i++)
    {
        for(j= 0; j < image->data->num_cols; j++)
        {
            pImage->pixels[i][j].r = image->data->pixels[i][j];
            pImage->pixels[i][j].g = image->data->pixels[i][j];
            pImage->pixels[i][j].b = image->data->pixels[i][j];
        }
    }
    /*will have to remove this... to do*/
    /*ow_horizontal_flip_image(pImage);*/

    ivi_write_image(pImage, file_name);

    ivi_free_image(pImage);
    return 0;
}

int read_camera_bw_byte_image(Camera_bw_byte_image **image, const char * file_name)
{

  EPETE(read_bw_byte_image( &((*image)->data),file_name));

  return NO_ERROR;
}

#ifdef __cplusplus
}
#endif
