/*
 * $Id: test_get_error.cpp 25499 2020-06-14 13:26:04Z kobus $
 */
#include <l/l_sys_std.h>
#include <l_cpp/l_exception.h>
#include <l_cpp/l_test.h>

int main(int, char**)
{
    const std::string gr="A screaming comes across the sky.";
    ivi_c::set_error(gr.c_str());
    std::string x = ivi::ivi_get_error();
    TEST_TRUE(x == gr);

    ivi_c::ivi_clear_error();
    x = ivi::ivi_get_error();
    TEST_TRUE(x.empty());

    const std::string pp="It is a truth universally acknowledged that...";
    ivi_c::add_error(pp.c_str());
    x = ivi::ivi_get_error();
    TEST_TRUE(x == pp);

    RETURN_VICTORIOUSLY();
}

