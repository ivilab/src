/**
 * @file
 * @brief simple demo of collage generation
 * @author Andrew Predoehl
 */
/*
 * $Id: test_collage.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <i_cpp/i_image.h>
#include <i_cpp/i_collage.h>
#include <l_cpp/l_util.h>

namespace {

int test1()
{
    // load a few images from ../input/
    const std::string path = std::string("..") + DIR_STR + "input" + DIR_STR;

    const ivi::Image    i2(path + "123_map.jpeg"),
                        i1(path + "123_trail.jpeg"),
                        i3(path + "test.jpeg"),
                        *images[4] = {&i1, &i2, &i3, &i2};

    // build a collage image
    ivi::Image c1(ivi::make_collage(images, 3, 1));
    ivi::Image c2(ivi::make_collage(images, 2, 2));
    ivi::Image c3(ivi::make_collage(images, 1, 3));

    // show it
    if (0 == ivi_c::ivi_fork())
    {
        c1.display("collage 3x1");
        c2.display("collage 2x2");
        c3.display("collage 1x3");
        while (1) {}
    }

    return ivi_c::NO_ERROR;
}

int test2()
{
    // load a few images from ../input/
    const std::string path = std::string("..") + DIR_STR + "input" + DIR_STR;

    ivi::Image    i2(path + "123_map.jpeg"),
                  i1(path + "123_trail.jpeg"),
                  i3(path + "test.jpeg");

    std::vector< ivi::Image > iii(4);
    iii.at(0).swap(i1);
    iii.at(1) = i2; // deep copy
    iii.at(2).swap(i3);
    iii.at(3).swap(i2);

    // build a collage image
    ivi::Image c1(ivi::make_collage(& iii.front(), 3, 1));
    ivi::Image c2(ivi::make_collage(& iii.front(), 2, 2));
    ivi::Image c3(ivi::make_collage(& iii.front(), 1, 3));

    // show it
    if (0 == ivi_c::ivi_fork())
    {
        c1.display("test2 3x1");
        c2.display("test2 2x2");
        c3.display("test2 1x3");
        while (1) {}
    }

    return ivi_c::NO_ERROR;
}


}

int main()
{
    try
    {
        IVI(EPETE(test1()));
        IVI(EPETE(test2()));
    }
    catch(const ivi::Exception& e)
    {
        e.print_details_exit();
    }
    return EXIT_SUCCESS;
}

