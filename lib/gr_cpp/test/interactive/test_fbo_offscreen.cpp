/* $Id: test_fbo_offscreen.cpp 25597 2020-06-30 23:31:45Z kobus $ */
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

#include <gr_cpp/gr_opengl.h>
#include <gr_cpp/gr_glut.h>
#include <gr_cpp/gr_fbo_offscreen.h>
#include "gr_cpp/gr_opengl_headers.h"



using namespace ivi::opengl;

int main(int argc, char** argv)
{
#ifdef IVI_HAVE_GLEW
    Glut_window wnd("Test fbo offscreen");
    Fbo_offscreen_buffer fbo(500, 500, false);
    Fbo_offscreen_buffer fbo(500, 500, false);
    fbo.bind();


    glClearColor(0.0, 1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    get_framebuffer_as_image(0, 0, 500, 500).write("fbo_test.jpg");
    fbo.unbind();

    return 0;
#else
    return ivi_c::ERROR;
#endif 
}
