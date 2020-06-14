/* $Id: l_readable.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef IVI_READABLE_H
#define IVI_READABLE_H

#include <l_cpp/l_exception.h>

#include <iosfwd>

namespace ivi
{

/**
 * @class Readable
 *
 * @brief Abstract class to read this object from an input stream.
 * @warning The three-parameter version of read_field_value is not thread-safe.
 */
class Readable
{
public:

    /** @brief Deletes this Readable. */
    virtual ~Readable() { }


    /** @brief Reads this Readable from an input stream. */
    virtual void read(std::istream& in) = 0;


    /** @brief Reads this Readable from a file. */
    virtual void read(const char* fname);


    /** 
     * @brief Reads a line off @em in into a buffer and returns a pointer
     * to the field value in the buffer.
     */
    static const char* read_field_value
    (
        std::istream& in,
        const char*   field_name, 
        char*         field_buf,
        size_t        buf_len,
        char          separator=':'
    );


    /** 
     * @brief Reads a line off @em in into an internal buffer and returns
     * a pointer to the field value in the buffer.
     * @warning This function is not thread-safe.
     */
    static const char* read_field_value
    (
        std::istream& in,
        const char*   field_name, 
        char          separator=':'
    );
};


}


#endif
