/* =========================================================================== *
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
   |  Author:  Jinyan Guan
 * =========================================================================== */

/* $Id: pt_face_2d_trajectory.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef PT_FACE_2D_TRAJECTORY_H
#define PT_FACE_2D_TRAJECTORY_H

#include <people_tracking_cpp/pt_complete_trajectory.h>
#include <people_tracking_cpp/pt_trajectory.h>
#include <people_tracking_cpp/pt_face_2d.h>

/** @brief   Classes and functions for dealing with trajectory files.  */

namespace ivi {
namespace pt {

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

typedef Generic_trajectory_map<Face_2d> Face_2d_trajectory_map;
typedef Generic_trajectory<Face_2d> Face_2d_trajectory;
typedef Generic_trajectory_element<Face_2d> Face_2d_trajectory_element;


/** @brief  Convert a Trajectory to a Face_2d_trajectory. */
Face_2d_trajectory get_face_2d_trajectory
(
    const Trajectory& traj,
    const Perspective_camera& cam
);

/** @brief  Convert a Trajectory_map to a Face_2d_trajectory_map. */
Face_2d_trajectory_map get_face_2d_trajectory_map
(
    const Trajectory_map& trajs,
    const Perspective_camera& cam
);

}}

namespace ivi {
namespace tracking {

/** @brief  Specialize this parse for Face_2d. */
template <>
inline
bool ivi::pt::Face_2d_trajectory_element::parse(const std::string& /*line*/)
{
//    using namespace std;
//    istringstream istr(line);
//    vector<double> elems;
//
//    copy(istream_iterator<double>(istr), istream_iterator<double>(),
//         back_inserter(elems));
//
//    IFT(elems.size() == 3, Runtime_error, 
//            "Cannot read trajectory element: line has wrong format.");
//
//    if(elems.back() == 0.0)
//        return false;
//
//    value = Face_2d(elems[0], elems[1]); 

    return true;
}

/** @brief  Specialize this write for Face_2d. */
template <>
inline
void ivi::pt::Face_2d_trajectory_element::write(std::ofstream& /*ofs*/) const
{
    //ofs << value << " 1.0" << std::endl;
}

/** @brief  Specialize this write_invalid for Face_2d. */
template <>
inline
void ivi::pt::Face_2d_trajectory_element::write_invalid(std::ofstream& ofs)
{
    ofs << "0.0 0.0 0.0" << std::endl;
}

}}

#endif /*PT_FACE_2D_TRACJECTORY_H */

