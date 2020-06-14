#include "gr_cpp/gr_rectangle_2d.h"
#include "gr_cpp/gr_opengl.h"
#include "gr_cpp/gr_opengl_headers.h"

namespace ivi {

std::vector<Vector> Rectangle_2d::get_corners() const
{
    std::vector<Vector> corners(4);
    corners[0].set(-m_width / 2.0, -m_height / 2.0);
    corners[1].set(-m_width / 2.0, m_height / 2.0);
    corners[2].set(m_width / 2.0, m_height / 2.0);
    corners[3].set(m_width / 2.0, -m_height / 2.0);

    Matrix R = ::ivi::geometry::get_rotation_matrix(m_orientation);
    for(size_t i = 0; i < corners.size(); i++)
    {
        corners[i] = m_center + R * (corners[i]);
    }

    return corners;
}

std::vector<Vector> Rectangle_2d::get_side_midpoints() const
{
    std::vector<Vector> midpoints(4);
    midpoints[0].set(m_width / 2.0, 0.0);
    midpoints[1].set(0.0, -m_height / 2.0);
    midpoints[2].set(-m_width / 2.0, 0.0);
    midpoints[3].set(0.0, m_height / 2.0);

    Matrix R = ::ivi::geometry::get_rotation_matrix(m_orientation);
    for(size_t i = 0; i < midpoints.size(); i++)
    {
        midpoints[i] = m_center + R * (midpoints[i]);
    }

    return midpoints;
}

void Rectangle_2d::wire_render() const
{
#ifdef IVI_HAVE_OPENGL
    std::vector<Vector> corners = get_corners();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
        ::ivi::opengl::glVertex(corners[0]);
        ::ivi::opengl::glVertex(corners[1]);
        ::ivi::opengl::glVertex(corners[2]);
        ::ivi::opengl::glVertex(corners[3]);
        ::ivi::opengl::glVertex(corners[0]);
    glEnd();
    glPopAttrib();

    GL_ETX();
#else
    IVI_THROW_2(Missing_dependency, "opengl");
#endif
}

} //namespace ivi

