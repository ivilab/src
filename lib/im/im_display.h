
/* $Id: im_display.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef IM_DISPLAY_INCLUDED
#define IM_DISPLAY_INCLUDED

#ifndef __C2MAN__

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#ifdef IVI_HAVE_X11
#include "magick/magick.h"
int im_display(Image* image, ImageInfo* image_info_ptr);
#endif 

int im_display_main(int argc, char** argv);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif
/* END Kobus */


#endif   /* #ifndef __C2MAN__ */

#endif   /* Include this file. */


