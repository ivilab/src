
/* $Id: im_display.h 25612 2020-07-06 22:17:23Z kobus $ */

#ifndef IM_DISPLAY_INCLUDED
#define IM_DISPLAY_INCLUDED

/* Start for c2man */

#ifndef __C2MAN__

/* End of includes for c2man */

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

/* End for c2man */

#endif   /* Include this file. */


