/**
 * @file
 * @brief test image scaling
 * @author'
 * @author Andy Predoehl
 */
/*
 * $Id: test_resize.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <i_cpp/i_image.h>

int main(int argc, char** argv)
{
    ivi::Image I("image.jpg");
    ivi::Image J = ivi::scale_image(I, 2.0);
    ivi::Image k = ivi::scale_image(I, 0.5);

    if (ivi_c::is_interactive())
    {
        I.display("original");
        J.display("double the height");
        k.display("half the height");
        ivi_c::nap(10*1000); /* ten seconds */
    }

    return EXIT_SUCCESS;
}
