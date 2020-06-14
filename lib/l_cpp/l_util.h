
/* $Id: l_util.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef IVI_WRAP_UTIL_H
#define IVI_WRAP_UTIL_H

#include <stdlib.h>

#define IVI(x) do {using namespace ivi_c; x;} while(0)

namespace ivi {

void alert_untested( int line, const char* file );

/** @brief  Counts the total number of elements in a 2D STL-style container. */
template<class C>
size_t length(const C& cner)
{
    size_t len = 0;
    for(typename C::const_iterator p = cner.begin(); p != cner.end(); ++p)
    {
        len += p->size();
    }

    return len;
}

}

#endif


