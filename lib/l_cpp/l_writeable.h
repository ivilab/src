/* $Id: l_writeable.h 25499 2020-06-14 13:26:04Z kobus $ */

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
|     Kyle Simek, Joseph Schlecht, Luca Del Pero
|
* =========================================================================== */

#ifndef IVI_WRITEABLE_H
#define IVI_WRITEABLE_H


#include <l_cpp/l_exception.h>
#include <iosfwd>



namespace ivi
{


/**
 * @class Writeable
 *
 * @brief Abstract class to write this object to an output stream.
 */
class Writeable
{
    public:

        /** @brief Deletes this Writeable. */
        virtual ~Writeable() { }


        /** @brief Writes this Writeable to an output stream. */
        virtual void write(std::ostream& out) const = 0;


        /** @brief Writes this Writeable to a file. */
        virtual void write(const char* fname) const;
};


}


#endif
