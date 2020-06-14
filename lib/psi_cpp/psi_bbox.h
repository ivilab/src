#ifndef IVI_PSI_BBOX_H
#define IVI_PSI_BBOX_H


/**
 * @file Various utility functions for converting 3D structures into 2D bouding boxes.
 */
#include <m_cpp/m_vector.h>
#include <camera_cpp/perspective_camera.h>
#include <g_cpp/g_util.h>
#include <g_cpp/g_cylinder.h>
#include <i_cpp/i_color_histogram.h>

#ifdef IVI_HAVE_UA_CARTWHEEL
#include <MathLib/Capsule.h>
#include <Control/CapsuleState.h>
#endif

#include <vector>
#include <psi_cpp/psi_weighted_box.h>
#include <psi_cpp/psi_util.h>

namespace ivi {
namespace psi {

#ifdef IVI_HAVE_UA_CARTWHEEL
typedef CartWheel::Math::Capsule Cw_capsule;
#endif

#ifdef IVI_HAVE_UA_CARTWHEEL
std::vector<Bbox>
get_bounding_boxes(
        const CartWheel::CapsuleState& capsules,
        const ivi::Perspective_camera& cam);
#endif

std::vector<Bbox>
get_bounding_boxes(
        const std::vector<ivi::Cylinder>& cylinders,
        const ivi::Perspective_camera& cam);


#ifdef IVI_HAVE_UA_CARTWHEEL
std::vector<ivi::Vector> capsules_to_3d_points(const std::vector<Cw_capsule*>& capsules);
#endif

std::vector<ivi::Vector> cylinder_to_3d_points(const ivi::Cylinder& c, double scale_long = 1.0, double angle_short = 0.0);

inline std::vector<ivi::Vector> box_to_3d_points(const Cuboid& box)
{
    return get_corners(box);
}


Bbox get_bounding_box(
        const std::vector<ivi::Vector>& points_3d,
        const ivi::Perspective_camera& cam);

#ifdef IVI_HAVE_UA_CARTWHEEL
inline Bbox get_bounding_box(const std::vector<Cw_capsule*>& capsules, const ivi::Perspective_camera& cam)
{
    std::vector<ivi::Vector> points_3d = capsules_to_3d_points(capsules);

    return get_bounding_box(points_3d, cam);
}
#endif

inline Bbox get_bounding_box(const ivi::Cylinder& cylinder, const ivi::Perspective_camera& cam, double scale_long = 1.0, double angle_short = 0.0)
{
    std::vector<ivi::Vector> points_3d = cylinder_to_3d_points(cylinder, scale_long, angle_short);

    return get_bounding_box(points_3d, cam);
}

inline Bbox get_bounding_box(const Cuboid box, const ivi::Perspective_camera& cam)
{
    std::vector<ivi::Vector> points_3d = box_to_3d_points(box);

    return get_bounding_box(points_3d, cam);
}


inline ivi::Vector project_point(const ivi::Perspective_camera& cam, const ivi::Vector& x)
{
    return ivi::geometry::projective_to_euclidean_2d(cam.build_camera_matrix() * ivi::geometry::euclidean_to_projective(x));
}

ivi::Color_histogram compute_color_histogram_from_box(const Bbox & box, const ivi::Image & img, unsigned int num_bins);


} // namespace psi
} // namespace ivi
#endif /*IVI_PSI_BBOX_H */

