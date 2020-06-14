/* $Id: gui_viewer_modes.h 25499 2020-06-14 13:26:04Z kobus $ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2012 by Kobus Barnard (author)
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


#ifdef IVI_HAVE_OPENGL 
#ifndef IVI_CPP_GUI_VIEWER_MODES_H
#define IVI_CPP_GUI_VIEWER_MODES_H

#include <gui_cpp/gui_viewer.h>
#include <l_cpp/l_index.h>

namespace ivi
{
namespace gui
{

/**
 * After enabling, viewer will treat the set of renderables as mutually exclusive; 
 * only one is displayed at a time.  Using '{' and '}' will cycle between the models.
 *
 * At present, adding or removing renderables after this is called isn't robustly supported.
 */
void enable_exclusive_display_mode(ivi::gui::Viewer& viewer, const ivi::Index_range& items);
 
} // namespace ivi
} // namespace gui

#endif
#endif /* IVI_HAVE_OPENGL  */
