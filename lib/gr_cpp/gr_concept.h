/* $Id: gr_concept.h 25499 2020-06-14 13:26:04Z kobus $ */
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

#ifndef IVI_GR_CPP_GR_CONCEPT_H
#define IVI_GR_CPP_GR_CONCEPT_H

#include <boost/concept_check.hpp> 

namespace ivi 
{
template <class X>
struct RenderableObject
{
    BOOST_CONCEPT_USAGE(RenderableObject)
    {
        render(*i);
    }
private:
    const X* i;
};
}

#endif
