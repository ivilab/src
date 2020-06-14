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

/* $Id: flow_dense.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef IVI_FLOW_DENSE_H
#define IVI_FLOW_DENSE_H

#include <m_cpp/m_matrix.h>
#include <gr_cpp/gr_2D_bounding_box.h>

#include <string>
#include <vector>

namespace ivi
{

/**
 * @brief Get the average optical flow inside a rectangle region
 *
 */
Vector average_flow
(
    const Matrix& x_flows, 
    const Matrix& y_flows,
    const Axis_aligned_rectangle_2d& roi
);

/** 
 * @brief Compute the flow magnitude 
 *
 */
Matrix flow_magnitude(const Matrix& x_flows, const Matrix& y_flows);

} // namespace ivi

#endif
