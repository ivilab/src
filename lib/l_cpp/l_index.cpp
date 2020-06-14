/* $Id: l_index.cpp 25499 2020-06-14 13:26:04Z kobus $ */
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

#include <l_cpp/l_index.h>
#include <l_cpp/l_int_vector.h>

namespace ivi {
const Index_range Index_range::ALL = Index_range(true);

Index_range::Index_range(const Int_vector& v) :
    ranges_(),
    all_(false)
{
    ranges_.push_back(new Listing_element(v));
}

Index_range::Listing_element::Listing_element(Int_vector v) :
    indices_(v.begin(), v.end())
{}

}

