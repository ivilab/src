
/* $Id: im_score.h 25612 2020-07-06 22:17:23Z kobus $ */

#ifndef IM_SCORE_INCLUDED
#define IM_SCORE_INCLUDED

/* Start for c2man */

#ifndef __C2MAN__ 

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

int im_score_main(int argc, char** argv);

#ifdef IVI_HAVE_X11
int im_score(Image*, ImageInfo*);
#endif 

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif


#endif   /* #ifndef __C2MAN__ */

/* End for c2man */

#endif   /* Include this file. */



