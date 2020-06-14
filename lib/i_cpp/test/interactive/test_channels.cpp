/**
 * @file
 * @brief unit test for the get channel function
 * @author Andrew Predoehl
 */
/*
 * $Id: test_channels.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <i_cpp/i_image.h>

namespace {

int nnn = 0;

void show(const ivi::Image& iii)
{
    std::string title = "Image 0";
    title[6] += nnn++;

    if (ivi_c::ivi_fork()) return;
    iii.display(title);
    while(1) {}
}

int test()
{
    ivi::Image iii("image.jpg");
    const ivi::Matrix   mr(iii.get_channel(ivi::Image::RED)),
                        mg(iii.get_channel(ivi::Image::GREEN)),
                        mb(iii.get_channel(ivi::Image::BLUE)),
                        *chan[3] = {&mr, &mg, &mb};
    show(iii);

    for (int ccc = ivi::Image::RED; ccc < ivi::Image::END_CHANNELS; ++ccc)
    {
        ivi::Image jjj(*chan[ccc]);
        show(jjj);
    }

    return 0;
}


}

int main()
{
    try
    {
        return test();
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }
}
