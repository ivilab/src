/* $Id: video_io.h 25499 2020-06-14 13:26:04Z kobus $ */
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

#ifndef IVI_CPP_VIDEO_IO_H
#define IVI_CPP_VIDEO_IO_H

#include <boost/function.hpp>
#include <string>

namespace ivi
{

namespace opengl
{

/**
 * Generate a set of video frames using opengl.
 */
void generate_video_frames(
    const boost::function0<void>& draw,
    const boost::function0<void>& tick,
    const size_t num_frames,
    const std::string& frame_out_fmt);

/**
 * Generate a set of video frames using opengl.
 * TODO
 */
//void generate_video(
//    const boost::function0<void>& draw,
//    const boost::function0<void>& tick,
//    size_t num_frames,
//    float frame_rate,
//    const std::string& out_fname);

} // namespace opengl;
} // namespace ivi

#endif
