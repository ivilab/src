/*
 * $Id: test_filter.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include "i_cpp/i_image.h"
#include "i_cpp/i_filter.h"
#include "m_cpp/m_matrix.h"
#include "i/i_display.h"

int main(int argc, char** argv)
{
    try
    {
        ivi::Matrix M(5, 5);
        ivi::Filter K1(5, 5);

        for (int i = 0; i < M.get_num_rows(); i++)
        {
            for (int j = 0; j < M.get_num_cols(); j++)
            {
                M.at(i, j) = i * j;
                K1.at(i, j) = i * j;
            }
        }

        ivi::Filter K2(M);

        ivi::Image I1("image.jpg");
        ivi::Filter K3 = ivi::gaussian_filter(9, 3);
        ivi::Image I2 = I1 * K3;
        ivi::Filter K4 = ivi::laplacian_of_gaussian_filter(19, 0.5);
        ivi::Image I3 = I1 * K4;
        ivi::Image i4(ivi::get_inverted(I1));

        if (ivi_c::is_interactive())
        {
            int wh[4];
            wh[0] = I1.display("original image");
            wh[1] = I2.display("blurred image");
            wh[2] = I3.display("log 0.01");
            wh[3] = i4.display("inverted image");

            ivi_c::nap(10*1000); // ten seconds
            ivi_c::close_displayed_image(wh[0]);
            ivi_c::close_displayed_image(wh[1]);
            ivi_c::close_displayed_image(wh[2]);
            ivi_c::close_displayed_image(wh[3]);
        }
    }
    catch (ivi::Exception& e)
    {
        e.print_details_exit();
    }

    return EXIT_SUCCESS;
}

