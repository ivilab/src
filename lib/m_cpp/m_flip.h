/* $Id: m_flip.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef IVI_CPP_EDGE_FLIP_H
#define IVI_CPP_EDGE_FLIP_H

#include "m_cpp/m_matrix.h"

namespace ivi
{

/// @brief deprecated synonym for Matrix::ow_vertical_flip
inline void flip_matrix_ud(Matrix& m)
{
    m.ow_vertical_flip();
}

/// @brief deprecated synonym for Matrix::ow_horizontal_flip
inline void flip_matrix_lr(Matrix& m)
{
    m.ow_horizontal_flip();
}

} // namespace ivi

#endif
