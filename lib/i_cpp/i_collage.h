/**
 * @file
 * @brief wrapper for ivi_c function to make a collage
 */
/*
 * $Id: i_collage.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef I_CPP_I_COLLAGE_H_IVI_LAB_LIBRARY
#define I_CPP_I_COLLAGE_H_IVI_LAB_LIBRARY

#include <i/i_collage.h>
namespace ivi {

class Image;

/**
 * @addtogroup iviImageProc
 * @{
 */

/// @brief wrapper for ivi_c::make_image_collage
Image make_collage(
    Image const* const* input_images,
    int num_horizontal,
    int num_vertical
);

/// @brief also works for an array of Image objects
Image make_collage(
    Image const* input_img_array,
    int num_horizontal,
    int num_vertical
);

/// @}

}

#endif /* I_CPP_I_COLLAGE_H_IVI_LAB_LIBRARY */
