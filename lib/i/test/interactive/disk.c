/*
 * $Id: disk.c 25592 2020-06-29 19:12:01Z kobus $ 
 */

#include <i/i_float.h>
#include <i/i_draw.h>
#include <i/i_float_io.h>

int main(void)
{
    IVI_image *i = NULL;

    Pixel p;
    p.r = p.g = p.b = 255.0f;

    EPETE(get_initialized_image(&i, 100, 100, &p));
    NPETE(i);

    EPETE(image_draw_disk(i, -5, -5, 20, 100, 0, 0));
    EPETE(image_draw_disk(i, 50, -10, 20, 80, 80, 0));
    EPETE(image_draw_disk(i, 100, -20, 20, 10, 100, 0));
    EPETE(image_draw_disk(i, 110, 50, 20, 10, 80, 80));
    EPETE(image_draw_disk(i, 80, 110, 20, 0, 10, 100));
    EPETE(image_draw_disk(i, 60, 90, 20, 80, 10, 80));
    EPETE(image_draw_disk(i, 10, 80, 20, 100, 10, 50));
    EPETE(image_draw_disk(i, 0, 50, 20, 50, 50, 50));

    EPETE(image_draw_disk(i, 50, 50, 10, 50, 50, 50));
    EPETE(image_draw_disk(i, 50, 50, 4, 100, 100, 100));
    EPETE(image_draw_disk(i, 50, 50, 1, 0, 0, 0));

    EPETE(ivi_write_image(i, "disk_image.tif"));
    ivi_free_image(i);

    return 0;
}
