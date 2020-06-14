/* $Id: test_viewer.cpp 25499 2020-06-14 13:26:04Z kobus $ */
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

#include <gr_cpp/gr_glut.h>
#include <gui_cpp/gui_viewer.h>

void render_teapot_2()
{
    glPushMatrix();
    glTranslatef(-3, 0, 0);
    ivi::opengl::Teapot().render();
    glPopMatrix();
}

int main()
{
    ivi::opengl::Glut_window wnd;
    ivi::gui::Viewer viewer(800, 600);
    viewer.attach(wnd);

    ivi::Perspective_camera cam(1, 1000);
    cam.set_camera_centre(ivi::Vector(0, -3, 10));
    ivi::opengl::Teapot teapot;
    viewer.add_renderable(&teapot);
    viewer.add_render_callback(render_teapot_2);

    viewer.set_rotation_origin(ivi::Vector3(-3, 0, 0));
    viewer.set_camera(cam);


    glutMainLoop();
}
