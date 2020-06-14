/* $Id: l_cloneable.h 25499 2020-06-14 13:26:04Z kobus $ */

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
|     Joseph Schlecht, Luca Del Pero
|
* =========================================================================== */


/**
 * @file
 *
 * @author Joseph Schlecht, Luca Del Pero
 *
 * @brief Abstract class to clone this object.
 */


#ifndef IVI_CLONEABLE_H
#define IVI_CLONEABLE_H


namespace ivi {


/**
 * @class Cloneable
 *
 * @brief Abstract class to clone this object.
 */
class Cloneable
{
    public:

        /** @brief Deletes this Cloneable. */
        virtual ~Cloneable() { }


        /** @brief Processes an Option from program command-line arguments. */
        virtual Cloneable* clone() const = 0;
};


}


#endif
