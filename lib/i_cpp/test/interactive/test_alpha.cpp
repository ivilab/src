/**
 * @file
 * @brief test prog demonstrating use of alpha channel
 * @author Andrew Predoehl
 */
/*
 * $Id: test_alpha.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <i_cpp/i_image.h>
#include <i_cpp/i_pixel.h>

int main (int argc, char** argv)
{
    ivi::Image i(100, 100, 0, 0, 0), j(i);

    ivi::enable_transparency(i);
    ivi::enable_transparency(j);

    ivi::PixelRGBA p(200, 0, 0, 255), q(0, 200, 0, 255);

    /* draw fading rectangles */
    for (int a = 0; a < 50; ++a)
    {
        q.extra.alpha = p.extra.alpha = 255 - 5*a;
        for (int b = 0; b < 50; ++b)
        {
            i( a+25, b+25 ) = p;
            j( b+40, a+40 ) = q;
        }
    }

    i.write("foo_image.tif");
    j.write("bar_image.tif");

    return 0;
}
