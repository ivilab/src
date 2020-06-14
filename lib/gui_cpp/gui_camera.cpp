/* $Id: gui_camera.cpp 25499 2020-06-14 13:26:04Z kobus $ */
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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <camera_cpp/perspective_camera.h>
#include <gui_cpp/gui_camera.h>
#include <gr_cpp/gr_frustum.h>
#include <boost/optional.hpp>
#include <i_cpp/i_image.h>

namespace ivi
{
namespace gui
{

struct Camera_key_listener
{
    bool operator()(unsigned int k, int, int)
    {
        int i = k - '1';
        if(i == -1) // wrap '0' to 10th index instead of first
            i = 9;

        if(cams.size() > 10)
            i = (int)(i * cams.size() / 10.0);

        if(i < 0 || i >= cams.size()) return false;

        viewer->animate_camera(cams[i], msec);
        return true;
    }

    std::vector<ivi::Perspective_camera> cams;
    Viewer* viewer;
    size_t msec;
};


/**
 * Add key listener that will switch between cameras when a number-key is pressed.
 * Transitions will be animated by smoothly transitioning from the current camera to the
 * destination camera. Number keys correspond to index in the cams vector, starting
 * with '1' and ending with '0'.  If cams.size() > 10, a subset of the cameras
 * will be used, chosen to be evenly spaced along the cams vector.
 *
 * Cameras will be copied into the callback object before passing it to the viewer
 *
 * @param msec number of milliseconds that the transition should take
 * @return an iterator to the listener callback as it is stored in the viewer.  This can be used to remove the listener later if needed
 */
Viewer::Keyboard_listener_iterator
add_camera_keys(ivi::gui::Viewer& viewer, const std::vector<ivi::Perspective_camera>& cams, int msec)
{
    boost::shared_ptr<Camera_key_listener> listener(new Camera_key_listener());
    listener->cams = cams;
    listener->viewer = &viewer;
    listener->msec = msec;
    return viewer.add_keyboard_listener(boost::bind(&Camera_key_listener::operator(), listener, _1, _2, _3));
}


void add_camera_frusta_(
        ivi::gui::Viewer& viewer,
        const std::vector<ivi::Perspective_camera>& cams,
        int width,
        int height,
        double scale,
        boost::optional<const std::vector<ivi::Image>& > img)
{
    for(size_t i = 0; i < cams.size(); ++i)
    {
        ivi::opengl::Frustum_display frustum(
                cams[i].get_camera_centre(),
                cams[i].build_camera_matrix(),
                width,
                height,
                scale, 0.0);
        if(img)
            frustum.set_image((*img)[i]);

        viewer.copy_renderable(frustum);
    }
}

/**
 * Add visualization for camera frusta.  This is a convienience
 * function that simply constructs ivi::opengl::Frustum_display objects and 
 * adds them to the viewer's list of renderables.  If you
 * want to do anything more fancy than displaying static cameras
 * (e.g. show camera trajectories over time) you're better off doing this 
 * manually.
 */
void add_camera_frusta(
        ivi::gui::Viewer& viewer,
        const std::vector<ivi::Perspective_camera>& cams,
        int width,
        int height,
        double scale)
{
    add_camera_frusta_(viewer, cams, width, height, scale, boost::none);
}

void add_camera_frusta(
        ivi::gui::Viewer& viewer,
        const std::vector<ivi::Perspective_camera>& cams,
        const std::vector<ivi::Image>& img,
        double scale)
{
    if(cams.size() != img.size())
        IVI_THROW(ivi::Dimension_mismatch);
    assert(img.size() > 0);
    add_camera_frusta_(viewer, cams, img[0].get_num_cols(), img[0].get_num_rows(), scale, img);
}

}
}
#endif // IVI_HAVE_OPENGL
