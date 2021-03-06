/* $Id: l_test.cpp 25499 2020-06-14 13:26:04Z kobus $ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2014 by Kobus Barnard (author)
   |
   |  Personal and educational use of this code is granted, provided that this
   |  header is kept intact, and that the authorship is not misrepresented, that
   |  its use is acknowledged in publications, and relevant papers are cited.
   |
   |  For other use contact the author (kobus AT cs DOT arizona DOT edu).
   |
   |  Please note that the code in this file has not necessarily been adequately
   |  tested. Naturally, there is no guarantee of performance, support, or fitness
   |  for any particular task. Nonetheless, I am interested in hearing about
   |  problems that you encounter.
   |
   |  Author:  Kyle Simek
 * =========================================================================== }}}*/

// vim: tabstop=4 shiftwidth=4 foldmethod=marker

#include <l/l_error.h>
#include <l/l_global.h>
#include <l_cpp/l_test.h>
#include <l_cpp/l_util.h>

#include <iostream>
#include <sstream>

void down_in_flames(
    const char* test_kind,
    const char* filename,
    int line_num,
    const char* bad_line
)
{
    std::cerr << filename << ':' << line_num << ":  " << test_kind
              << " EPIC FAIL whilst executing\n\t\"" << bad_line << "\"\n";
    // changed from ivi_exit() to abort() for easier debugging
    ivi_c::ivi_abort();
}

void print_victory(const char* filename)
{
    std::cout << filename << ":  success\n";
}

int scan_time_factor(const char* argv1, int* tf)
{
    // No time factor?  That is fine.  *tf remains at its default value.
    if (00 == argv1) return ivi_c::NO_ERROR;

    // No output parameter?  That is a problem.
    IVI(NRE(tf));

    // try to scan an integer from argv1
    int time_factor = -1;
    std::istringstream s(argv1);
    if (!(s >> time_factor))
    {
        ivi_c::set_error("Bad time factor string: %s", argv1);
        if ('\0' == argv1[0]) ivi_c::add_error("(empty string)");
        ivi_c::add_error("Time factor should be nonnegative or omitted.");
        return ivi_c::ERROR;
    }

    // test scanned value
    if (time_factor < 0)
    {
        ivi_c::set_error("Bad time factor value (must be nonnegative): %d",
                            time_factor);
        return ivi_c::ERROR;
    }

    // look for trailing characters
    char c;
    if (s >> c)
    {
        ivi_c::set_error("Time factor string has trailing characters: %s",
                          argv1);
        return ivi_c::ERROR;
    }

    *tf = time_factor;
    return ivi_c::NO_ERROR;
}

