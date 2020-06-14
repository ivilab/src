/*
 * $Id: transparent.c 25499 2020-06-14 13:26:04Z kobus $ 
 */

#include <i/i_float.h>
#include <i/i_draw.h>
#include <i/i_float_io.h>

#define HAS_TRANSPARENCY 1

int main(int argc, char** argv)
{
    IVI_image *i = NULL, *j = NULL;
    int a, b;

    Pixel p;
    p.r = p.g = p.b = 0.0f;
#if HAS_TRANSPARENCY
    p.extra.alpha = 0.0;
#else
    p.extra.invalid.pixel = VALID_PIXEL;
#endif

    EPETE(get_initialized_image(&i, 100, 100, &p));
    EPETE(get_initialized_image(&j, 100, 100, &p));
    NPETE(i);
    NPETE(j);

#if HAS_TRANSPARENCY
    i -> flags |= HAS_ALPHA_CHANNEL;
    j -> flags |= HAS_ALPHA_CHANNEL;
#endif

    EPETE(image_draw_rectangle(i, 25, 25, 50, 50, 200, 0, 0));
    EPETE(image_draw_rectangle(j, 40, 40, 50, 50, 0, 200, 0));

    /* add some blending */
    for (a = 0; a < 50; ++a)
    {
        int alpha = 255 - 5*a;
        for (b = 0; b < 50; ++b)
        {
            i -> pixels[ a+25 ][ b+25 ].extra.alpha = alpha;
            j -> pixels[ b+40 ][ a+40 ].extra.alpha = alpha;
        }
    }

    EPETE(ivi_write_image(i, "foo_image.tif"));
    EPETE(ivi_write_image(j, "bar_image.tif"));
    ivi_free_image(i);
    ivi_free_image(j);

    return 0;
}
