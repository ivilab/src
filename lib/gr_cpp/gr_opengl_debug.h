/* $Id: gr_opengl_debug.h 25499 2020-06-14 13:26:04Z kobus $ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
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

#ifndef IVI_CPP_GR_OPENGL_DEBUG
#define IVI_CPP_GR_OPENGL_DEBUG

namespace ivi
{
namespace opengl
{

// Note: These files are _intentionally_ in the global namespace to simplify calling from gdb.
// Function name prefixes mimick actual IVI namespaces

/** @brief Pop up a window displaying the opengl color buffer */
void ivi_opengl_debug_display_buffer();

}
}
#endif
