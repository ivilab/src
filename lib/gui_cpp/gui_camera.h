/* $Id: gui_camera.h 25499 2020-06-14 13:26:04Z kobus $ */
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
#ifndef IVI_CPP_GUI_CAMERA_H
#define IVI_CPP_GUI_CAMERA_H

#include <camera_cpp/perspective_camera.h>
#include <gui_cpp/gui_viewer.h>
#include <vector>

namespace ivi
{
namespace gui
{

/**
 * @brief Add a set of cameras to a viewer, which can be transitioned between using the
 * number keys 
 * */
Viewer::Keyboard_listener_iterator
add_camera_keys(ivi::gui::Viewer& viewer, const std::vector<ivi::Perspective_camera>& cams, int msec = 500);

/**
 * @brief Add visualization for camera frusta
 */
void add_camera_frusta(
        ivi::gui::Viewer& viewer,
        const std::vector<ivi::Perspective_camera>& cams,
        int width,
        int height,
        double scale);

/**
 * @brief Add visualization for camera frusta w/ images
 */
void add_camera_frusta(
        ivi::gui::Viewer& viewer,
        const std::vector<ivi::Perspective_camera>& cams,
        const std::vector<ivi::Image>& img,
        double scale);

}
}
#endif
#endif /* IVI_HAVE_OPENGL */
