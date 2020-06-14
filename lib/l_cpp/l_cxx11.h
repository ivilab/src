/* $Id$ */
/* {{{=========================================================================== *
   |
   |  Copyright (c) 1994-2014 by Kobus Barnard (author)
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

#ifndef IVI_L_CPP_L_CXX11_H
#define IVI_L_CPP_L_CXX11_H

#ifdef IVI_HAVE_CXX11
#include <utility>
#include <type_traits>
namespace ivi { // passthrough
    template <typename T> 
    inline typename std::remove_reference<T>::type&& move(T&& v) 
    {
        return std::move(v); 
    }

#ifdef IVI_HAVE_CXX14
    template <typename T> 
    inline constexpr typename  std::remove_reference<T>::type&& move(T&& v) 
    { 
        return std::move(v); 
    }
#endif /* IVI_HAVE_CXX14 */
}

#define IVI_STATIC_ASSERT(x,y) static_assert(x,y)

#else /* !IVI_HAVE_CXX11 */
namespace ivi { // C++11 emulation
    template <typename T>
    inline T& move(T& v) { return v; }

    template <typename T>
    inline const T& move(const T& v) { return v; }
}

#define IVI_STATIC_ASSERT(x,y) typedef char __STATIC_ASSERT__[( x )?1:-1]
#endif /* IVI_HAVE_CXX11 */

#endif /* IVI_L_CPP_L_CXX11_H */
