/**
 * @file
 * @author Andrew Predoehl
 * @brief unit test for function ivi::realpath()
 */
/*
 * $Id: real_path.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_std.h>
#include <l/l_sys_lib.h>
#include <l/l_sys_io.h>
#include <l/l_init.h>
#include <l/l_error.h>
#include <l/l_global.h>
#include <l_cpp/l_filesystem.h>
#include <l_cpp/l_exception.h>
#include <l_cpp/l_util.h>

#define SHOULD_BE_TRUE(b) check((b), __FILE__, __LINE__)

namespace {

void check(bool false_means_failure, const char* file, unsigned line)
{
    if (false_means_failure == false)
    {
        ivi_c::ivi_fprintf(stderr, "Failure %s:%u\n", file, line);
        exit(EXIT_FAILURE);
    }
}

int test()
{
    const char  *p1 = "/usr/bin",
                *p2 = "/usr/bin/",
                *p3 = "/usr/lib",
                *p4 = "/usr/lib/../lib",
                *p5 = "/usr/lib/../bin/";

    // hopefully no embarrassing typos
    SHOULD_BE_TRUE(ivi_c::is_directory(p1));
    SHOULD_BE_TRUE(ivi_c::is_directory(p2));
    SHOULD_BE_TRUE(ivi_c::is_directory(p3));
    SHOULD_BE_TRUE(ivi_c::is_directory(p4));
    SHOULD_BE_TRUE(ivi_c::is_directory(p5));

    // is it reflexive?
    SHOULD_BE_TRUE(ivi::realpath(p1) == ivi::realpath(p1));
    SHOULD_BE_TRUE(ivi::realpath(p2) == ivi::realpath(p2));
    SHOULD_BE_TRUE(ivi::realpath(p3) == ivi::realpath(p3));
    SHOULD_BE_TRUE(ivi::realpath(p4) == ivi::realpath(p4));
    SHOULD_BE_TRUE(ivi::realpath(p5) == ivi::realpath(p5));

    // is it symmetric?
    SHOULD_BE_TRUE(ivi::realpath(p1) == ivi::realpath(p2));
    SHOULD_BE_TRUE(ivi::realpath(p2) == ivi::realpath(p1)); // that's enough

    // is it gullible?
    SHOULD_BE_TRUE(ivi::realpath(p1) != ivi::realpath(p3));
    SHOULD_BE_TRUE(ivi::realpath(p1) != ivi::realpath(p4));
    SHOULD_BE_TRUE(ivi::realpath(p3) != ivi::realpath(p2));
    SHOULD_BE_TRUE(ivi::realpath(p4) != ivi::realpath(p2));

    // is it actually pretty smart?
    SHOULD_BE_TRUE(ivi::realpath(p3) == ivi::realpath(p4));
    SHOULD_BE_TRUE(ivi::realpath(p2) == ivi::realpath(p5));
    SHOULD_BE_TRUE(ivi::realpath(p1) == ivi::realpath(p5));
    SHOULD_BE_TRUE(ivi::realpath(p4) == ivi::realpath(p3));
    SHOULD_BE_TRUE(ivi::realpath(p5) == ivi::realpath(p2));

    return ivi_c::NO_ERROR;
}

}


int main()
{
    ivi_c::ivi_init();

    try
    {
        IVI(EPE(test()));
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }

    if (ivi_c::is_interactive()) ivi_c::ivi_printf("Success!\n");

    ivi_c::ivi_cleanup();

    return EXIT_SUCCESS;
}

