
/* $Id: im_score.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef IM_SCORE_INCLUDED
#define IM_SCORE_INCLUDED

#ifndef __C2MAN__ 

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

#endif   /* Include this file. */



