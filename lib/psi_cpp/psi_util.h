/* $Id: psi_util.h 25499 2020-06-14 13:26:04Z kobus $ */
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
#ifndef PSI_V1_UTIL
#define PSI_V1_UTIL

#include "l/l_sys_debug.h"  /* For ASSERT */
#include "m_cpp/m_vector.h"
#include "gr_cpp/gr_2D_bounding_box.h"
#include "camera_cpp/perspective_camera.h"
#include "g_cpp/g_quaternion.h"
#include "detector_cpp/d_deva_detection.h"
#include "people_tracking_cpp/pt_util.h"

#include "l/l_word_list.h"
#include "l/l_sys_io.h" /* for ivi_glob */

#ifdef IVI_HAVE_UA_CARTWHEEL
#include <MathLib/Point3d.h>
#include <MathLib/Vector3d.h>
#include <Core/Visualization.h>
#endif

namespace ivi
{
namespace psi
{

typedef ivi::Axis_aligned_rectangle_2d Bbox;

#ifdef IVI_HAVE_UA_CARTWHEEL
inline ivi::Vector to_ivi(const CartWheel::Math::Point3d& pt)
{
    return ivi::Vector(pt.x, pt.y, pt.z);
}

inline ivi::Vector to_ivi(const CartWheel::Math::Vector3d& vec)
{
    return ivi::Vector(vec.x, vec.y, vec.z);
}

inline ivi::Quaternion to_ivi(const CartWheel::Math::Quaternion& q)
{
    CartWheel::Math::Vector3d imag = q.getV();
    double real = q.getS();
    return ivi::Quaternion(imag.x, imag.y, imag.z, real);
}
#endif

#ifdef IVI_HAVE_UA_CARTWHEEL
inline CartWheel::Math::Point3d to_cw_pt_3d(const ivi::Vector& pt)
{
    ASSERT(pt.size() == 3);
    return CartWheel::Math::Point3d(pt[0], pt[1], pt[2]);

}
#endif

#ifdef IVI_HAVE_UA_CARTWHEEL
inline CartWheel::Math::Vector3d to_cw_vec_3d(const ivi::Vector& vec)
{
    ASSERT(vec.size() == 3);
    return CartWheel::Math::Vector3d(vec[0], vec[1], vec[2]);

}
#endif

#ifdef IVI_HAVE_UA_CARTWHEEL
/**
 * Pass a ivi perspective camera to a cartwheel visualization context
 *
 * @note Camera focal length is defined in units of screen pixels,
 * so we need screen size to convert it to fovy angle.
 */
void set_camera(CartWheel::Visualization& vis, const ivi::Perspective_camera& cam, double WIDTH, double HEIGHT);
#endif

/** /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ **/
inline ivi::Perspective_camera make_camera(
        double height,
        double tilt,
        double focal_length)
{
    ivi::Perspective_camera cam(0.1, 100);
    cam.set_camera_centre(ivi::Vector(0.0,height,0.0));
    cam.set_orientation(ivi::Quaternion(tilt, 0.0,0.0, ivi::Quaternion::XYZR));
    cam.set_focal_length(focal_length);

    return cam;
}

/**
 * @brief Project the 3D point x to image coordinate using camera matrix P
 *        and make the top left as the origin
 */
inline
Vector project_and_unstandarize(const Vector& x, const Matrix& P, double w, double h)
{
    using namespace geometry;
    Vector x2 = projective_to_euclidean_2d(P * euclidean_to_projective(x));
    pt::unstandardize(x2, w, h);
    return x2;
}


enum Simulator_type {CARTWHEEL_SIMULATOR, CYLINDER_SIMULATOR};

const std::string& get_name(Simulator_type type);

std::istream& operator>>(std::istream& ist, Simulator_type& type);
std::ostream& operator<<(std::ostream& ost, Simulator_type type);

/** @brief move coordinate ssystem origin to center of image  */
void standardize(Deva_detection& boxes, double cam_width, double cam_height);

/**
 * @brief   Prune the deva boxes based on the average entity height 
 */
void prune_by_height
(
    std::vector<Deva_detection>& deva_boxes,
    double screen_width,
    double screen_height,
    const Perspective_camera& camera, 
    double avereage_height
);

} // namespace psi
} // namespace ivi
#endif
