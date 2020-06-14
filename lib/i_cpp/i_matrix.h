/**
 * @file
 * @brief declaration of cpp wrapper for high-constract conversion fun
 * @author Andrew Predoehl
 */
/*
 * $Id: i_matrix.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef I_CPP_I_MATRIX_H_IVI_LAB_INCLUDED
#define I_CPP_I_MATRIX_H_IVI_LAB_INCLUDED

#include <i/i_matrix.h>
#include <m_cpp/m_matrix.h>
#include <i_cpp/i_image.h>
#include <i_cpp/i_colormap.h>

namespace ivi {

/**
 * @addtogroup iviImageProc
 * @{
 */

Image matrix_to_max_contrast_8bit_bw_image(const ivi::Matrix&);

/**
 * @brief Converts a matrix to a high-contrast color image, mapping values to
 *        colors using a ivi::Colormap.  Equivalent to Matlab's imagesc() function.
 */
Image matrix_to_color_image(const ivi::Matrix& m, const ivi::Colormap& map = "jet");

/** 
 * @brief Ported version of Matlab's "imagesc()" function; alias of 
 *        matrix_to_color_image */
inline Image imagesc(const ivi::Matrix& data, const ivi::Colormap& map = "jet")
{ 
    return matrix_to_color_image(data, map);
}

/// @}

}

#endif

