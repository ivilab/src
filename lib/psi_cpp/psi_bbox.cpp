#include "l/l_sys_debug.h"  /* For ASSERT */
#include "m_cpp/m_vector.h"
#include "g_cpp/g_quaternion.h"
#include "psi_cpp/psi_bbox.h"

#ifdef IVI_HAVE_UA_CARTWHEEL
#include <MathLib/Capsule.h>
#endif

#include <vector>

namespace ivi
{
namespace psi
{

#ifdef IVI_HAVE_UA_CARTWHEEL
typedef CartWheel::Math::Capsule Cw_capsule;
#endif

/// @param capsule If false, treat this like a cylinder
void append_cylinder_capsule_keypoints(
        const ivi::Vector& center1,
        const ivi::Vector& center2,
        double radius,
        bool capsule,
        std::vector<ivi::Vector>& result,
        size_t points_on_circle,
        double scale_long = 1.0,
        double angle_short = 0.0)
{
    ivi::Vector axis = center1 - center2;
    axis.normalize();

    if(capsule)
    {
        ivi::Vector end1 = center1 + axis * radius;
        ivi::Vector end2 = center2 - axis * radius;
        result.push_back(end1);
        result.push_back(end2);
    }
    else
    {
        ivi::Vector end1 = center1;
        ivi::Vector end2 = center2;
        result.push_back(end1);
        result.push_back(end2);
    }

    if(points_on_circle == 0)
    {
        return;
    }

    const double arc_size = (2 * M_PI) / points_on_circle;

    ivi::Quaternion q;
    q.set_from_directions(ivi::Vector(0.0,0.0,1.0), axis);

    ivi::Quaternion q2(ivi::Vector(0.0, 0.0, 1.0), -angle_short);

    for(size_t i = 0; i < points_on_circle; i++)
    {
        double angle = i * arc_size;
        ivi::Vector ref = ivi::Vector().set(scale_long * cos(angle), sin(angle), 0.0);
        ref *= radius;

        ivi::Vector x = q.rotate(q2.rotate(ref));

        result.push_back(center1 + x);
        result.push_back(center2 + x);
    }
}

inline void append_cylinder_keypoints(
        const ivi::Cylinder& cylinder,
        std::vector<ivi::Vector>& result,
        size_t points_on_circle,
        double scale_long = 1.0,
        double angle_short = 0.0)
{
    return append_cylinder_capsule_keypoints(
            cylinder.get_p1(),
            cylinder.get_p2(),
            cylinder.get_radius(),
            false,
            result,
            points_on_circle,
            scale_long,
            angle_short);
}

inline void append_capsule_keypoints(
        const ivi::Vector& center1,
        const ivi::Vector& center2,
        double radius,
        std::vector<ivi::Vector>& result,
        size_t points_on_circle)
{
    return append_cylinder_capsule_keypoints(
            center1,
            center2,
            radius,
            true,
            result,
            points_on_circle);
}

#ifdef IVI_HAVE_UA_CARTWHEEL
void append_capsule_keypoints(const Cw_capsule* capsule, std::vector<ivi::Vector>& result, size_t points_on_circle)
{
    append_capsule_keypoints(
        ivi::Vector(capsule->p1.x, capsule->p1.y, capsule->p1.z),
        ivi::Vector(capsule->p2.x, capsule->p2.y, capsule->p2.z),
        capsule->radius,
        result, points_on_circle);
}
#endif


#ifdef IVI_HAVE_UA_CARTWHEEL
std::vector<ivi::Vector> capsules_to_3d_points(const std::vector<Cw_capsule*>& capsules)
{
//    const size_t poc = 4;
    const size_t poc = 0; // temporarilly ignore capsule width, because
        // cartwheel has a bug where it outputs a NaN for radius, which crashes thecode below.  KS April 8, 2011
    const size_t ppc = 2 * poc + 2;

    std::vector<ivi::Vector> points_3d;
    points_3d.reserve(capsules.size() * ppc);

    for(size_t i = 0; i < capsules.size(); i++)
    {
        Cw_capsule* capsule = capsules[i];

        // check for NaN
        if(
                capsule->p1.x != capsule->p1.x || 
                capsule->p1.y != capsule->p1.y || 
                capsule->p1.z != capsule->p1.z || 
                capsule->p2.x != capsule->p2.x || 
                capsule->p2.y != capsule->p2.y || 
                capsule->p2.z != capsule->p2.z || 
                capsule->radius != capsule->radius)
        {
            // corner case: cartwheel sometimes returns NaN for some capsules.
            continue;
        }

        append_capsule_keypoints(capsules[i], points_3d, poc);

        ASSERT(points_3d.size() > 0);
    }
    
    return points_3d;
}
#endif


std::vector<ivi::Vector> cylinder_to_3d_points(const ivi::Cylinder& cylinder, double scale_long, double angle_short)
{
    const size_t poc = 4;
    const size_t ppc = 2 * poc + 2;

    std::vector<ivi::Vector> points_3d;
    points_3d.reserve(ppc);
    append_cylinder_keypoints(cylinder, points_3d, poc, scale_long, angle_short);
    
    return points_3d;
}


Bbox get_bounding_box(
        const std::vector<ivi::Vector>& points_3d,
        const ivi::Perspective_camera& cam)
{
    std::vector<ivi::Vector> points_2d(points_3d.size());

    for(size_t i = 0; i < points_3d.size(); i++)
    {
        points_2d[i] = project_point(cam, points_3d[i]);

    }

    return compute_bounding_box(points_2d.begin(), points_2d.end());
}



#ifdef IVI_HAVE_UA_CARTWHEEL
std::vector<Bbox> get_bounding_boxes(
        const CartWheel::CapsuleState& capsules,
        const ivi::Perspective_camera& cam)
{
    std::vector<Bbox> boxes;

    for(int i = 0; i < capsules.getNumEntities(); i++)
    {
        boxes.push_back(get_bounding_box(capsules.getCapsules(i), cam));
    }

    return boxes;
}
#endif


std::vector<Bbox> get_bounding_boxes(
        const std::vector<ivi::Cylinder>& cylinders,
        const ivi::Perspective_camera& cam)
{
    std::vector<Bbox> boxes;

    for(size_t i = 0; i < cylinders.size(); i++)
    {
        boxes.push_back(get_bounding_box(cylinders[i], cam));
    }

    return boxes;
}

ivi::Color_histogram compute_color_histogram_from_box(const Bbox & box, const ivi::Image & img, unsigned int num_bins)
{
    return ivi::Color_histogram(num_bins, img, MAX_OF(0.0, box.get_left()), MAX_OF(0.0, box.get_bottom()),
            MIN_OF(box.get_right(), img.get_num_cols()-1), MIN_OF(box.get_top(),img.get_num_rows()-1) );
}

} // namespace psi
} // namespace ivi
