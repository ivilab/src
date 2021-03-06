/**
 * @file
 * @brief implementation of cpp wrapper for high-constract conversion fun
 * @author Andrew Predoehl
 */
/*
 * $Id: i_matrix.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include "l_cpp/l_exception.h"
#include "i_cpp/i_matrix.h"
#include "i/i_matrix.h"
#include "i_cpp/i_colormap.h"

#include <algorithm>

namespace ivi {

/**
 * @brief convert a matrix to a black-and-white image, with enhanced contrast.
 * @param m input matrix to visualize
 * @returns Image with all pixels adjusted for enhanced contrast
 *
 * This wraps the C function ivi_c::matrix_to_max_contrast_8bit_bw_image().
 */
Image matrix_to_max_contrast_8bit_bw_image(const ivi::Matrix& m)
{
    ivi_c::IVI_image *i = 0;
    // this does not leak, presuming an error return means no allocation.
    ETX(ivi_c::matrix_to_max_contrast_8bit_bw_image(m.get_c_matrix(), &i));
    return Image(i);
}

/**
 * Maps matrix values to [0,1], and transforms them to colors using the
 * provided colormap.  The following strings may be passed to specify 
 * preset colormaps, which are identical to those in Matlab:
 *  
 * | "jet" (default)
 * | "hot"
 * | "hsv"
 * | "gray"
 * | "cool"
 * | "lines"
 *
 * Using "gray" should give similar results to matrix_to_max_contast_8bit_bw_image()
 *
 * Custom colormaps may also by used by passing a ivi::Colormap object instead of 
 * as string.
 */
Image matrix_to_color_image(const ivi::Matrix& m, const ivi::Colormap& map)
{
    double max = ivi::max(m);
    double min = ivi::min(m);

    ivi::Matrix scaled_data(m);
    scaled_data.ow_add_scalar(-min);
    scaled_data /= (max - min);
    Image out(scaled_data.get_num_rows(), scaled_data.get_num_cols());

    std::transform(
        &scaled_data(0,0),
        &scaled_data(0,0) + scaled_data.get_length(),
        &out(0,0),
        map);

    return out;
}


}
