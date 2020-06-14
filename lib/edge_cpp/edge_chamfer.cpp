/* $Id: edge_chamfer.cpp 25499 2020-06-14 13:26:04Z kobus $ */
/* =========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
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
 * =========================================================================== */

#include <edge_cpp/edge_chamfer.h>

namespace ivi {
Chamfer_transform::Chamfer_transform(const Self& other) :
    m_size(other.m_size),
    m_edges(other.m_edges),
    m_num_rows(other.m_num_rows),
    m_num_cols(other.m_num_cols),
    m_distances(other.m_distances),
    m_edge_map(other.m_edge_map)
{
}
} // namespace ivi
