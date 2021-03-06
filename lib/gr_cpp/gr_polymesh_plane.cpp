/* $Id$ */
/**
 * This work is licensed under a Creative Commons 
 * Attribution-Noncommercial-Share Alike 3.0 United States License.
 * 
 *    http://creativecommons.org/licenses/by-nc-sa/3.0/us/
 * 
 * You are free:
 * 
 *    to Share - to copy, distribute, display, and perform the work
 *    to Remix - to make derivative works
 * 
 * Under the following conditions:
 * 
 *    Attribution. You must attribute the work in the manner specified by the
 *    author or licensor (but not in any way that suggests that they endorse you
 *    or your use of the work).
 * 
 *    Noncommercial. You may not use this work for commercial purposes.
 * 
 *    Share Alike. If you alter, transform, or build upon this work, you may
 *    distribute the resulting work only under the same or similar license to
 *    this one.
 * 
 * For any reuse or distribution, you must make clear to others the license
 * terms of this work. The best way to do this is with a link to this web page.
 * 
 * Any of the above conditions can be waived if you get permission from the
 * copyright holder.
 * 
 * Apart from the remix rights granted under this license, nothing in this
 * license impairs or restricts the author's moral rights.
 */

/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author).
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
| Authors:
|     Emily Hartley
|
* =========================================================================== */

#include "gr_cpp/gr_polymesh_plane.h"

using namespace ivi;

/**
 * @param p  the polymesh
 */
Polymesh_Plane::Polymesh_Plane(const Polymesh& p)
{
    m_p = &p;
}

/**
 * @param p  the polymesh
 * @param params  the vector of coefficients of the plane
 * @param indices  the vector of indices of the faces in the polymesh p
 *                 that lie in the plane defined by params
 */
Polymesh_Plane::Polymesh_Plane(const Polymesh& p, Vector& params, std::vector<int>& indices)
{
    m_p = &p;
    plane_params = params;
    face_indices = indices;
}

/**
 * @param pp  the Polymesh_Plane to copy into this one.
 */
Polymesh_Plane::Polymesh_Plane(const Polymesh_Plane& pp)
{
    this->m_p = pp.get_polymesh();
    this->plane_params = pp.get_plane_params();
    this->face_indices = pp.get_face_indices();
}

/** Frees all space allocated by this Polymesh_Plane. */
Polymesh_Plane::~Polymesh_Plane()
{
}

/**
 * Performs a deep copy of the plane parameters and face indices.
 *
 * @param pp  Polymesh_Plane to copy into this one.
 *
 * @return A reference to this Polymesh_Plane.
 */
Polymesh_Plane& Polymesh_Plane::operator=(const Polymesh_Plane& pp)
{
    if(this == &pp) return *this;

    this->m_p = pp.get_polymesh();
    this->plane_params = pp.get_plane_params();
    this->face_indices = pp.get_face_indices();

    return *this;
}

