/**
 * @file
 * @brief Class def. for deprecated Opengl framebuffer
 * @author Kyle Simek
 */
/*
 * $Id: i_opengl_buf.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef IVI_CPP_OPENGL_IMAGE_BUF_H
#define IVI_CPP_OPENGL_IMAGE_BUF_H

#include <i_cpp/i_image.h>

namespace ivi
{

/**
 * @ingroup iviImageProc Image Processing
 */

/**
 * Class for constructing an image from the opengl framebuffer
 * @note This method is DEPRECATED in favor of the ivi::opengl::framebuffer_to_image() function.
 * @deprecated
 *
 * Do not use this class in new code.
 */
class Opengl_framebuffer_image : public Image
{
public:
//    IVI_LEGACY(Opengl_framebuffer_image(uint32_t x, uint32_t y, uint32_t height, uint32_t width));
    Opengl_framebuffer_image(uint32_t x, uint32_t y, uint32_t height, uint32_t width);
};


} //namespace ivi

#endif /* IVI_CPP_OPENGL_IMAGE_BUF_H */
