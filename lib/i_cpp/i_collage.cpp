/**
 * @file
 * @brief wrapper for ivi_c function to make a collage
 */
/*
 * $Id: i_collage.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include "i_cpp/i_collage.h"
#include "i_cpp/i_image.h"
#include "l_cpp/l_exception.h"

#include <vector>

namespace {

size_t positive_product(int h, int v)
{
    if (h < 1 || v <1)
    {
        IVI_THROW_2(ivi::Illegal_argument, "Collage sizes must be positive");
    }
    return h*v;
}

}

namespace ivi {

Image make_collage(
    Image const* const* input_images,
    int num_horizontal,
    int num_vertical
)
{
    const size_t count = positive_product(num_horizontal, num_vertical);

    ivi_c::IVI_image* output = 0;
    std::vector<const ivi_c::IVI_image*> i2(count);
    for (size_t iii = 0; iii < count; ++iii)
    {
        i2.at(iii) = input_images[iii] -> c_ptr();
    }

    ETX(ivi_c::make_image_collage(&output, num_horizontal, num_vertical,
                                                    & i2.front()));
    return Image(output);
}


Image make_collage(
    Image const* input_img_array,
    int num_horizontal,
    int num_vertical
)
{
    const size_t count = positive_product(num_horizontal, num_vertical);

    std::vector< const ivi::Image* > in(count, 0);
    for (size_t iii = 0; iii < count; ++iii)
    {
        in.at(iii) = input_img_array + iii;
    }
    return make_collage(& in.front(), num_horizontal, num_vertical);
}

}

