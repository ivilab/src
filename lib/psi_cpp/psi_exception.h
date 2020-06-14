/* $Id: psi_exception.h 25499 2020-06-14 13:26:04Z kobus $ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2011 by Kobus Barnard (author)
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

#include <l_cpp/l_exception.h>

namespace ivi
{
namespace psi
{

class File_format_exception : public IO_error
{
public:
    File_format_exception(const char* file, uint32_t line) : IO_error("File_format_Exception", file, line)
    {}

    File_format_exception(const char* msg, const char* file=0, uint32_t line=0) :
        IO_error(msg, file, line)
    {}


    File_format_exception(const std::string& msg, const char* file=0, uint32_t line=0) :
        IO_error(msg, file, line)
    {}

    virtual ~File_format_exception() { }
};

}
}
