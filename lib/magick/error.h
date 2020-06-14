
/* $Id: error.h 25499 2020-06-14 13:26:04Z kobus $ */

#ifndef __C2MAN__     

/*
 * Make sure that these files are not a liability when there is no X11. If this
 * is the case, then comment out the whole thing.
*/
#ifdef IVI_HAVE_X11 

/* -------------------------------------------------------------------------- */


#ifndef MAGICK_ERROR_INCLUDED   /* Kobus */
#define MAGICK_ERROR_INCLUDED   /* Kobus */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/*
  Error typedef declarations.
*/
typedef void
  (*ErrorHandler) _Declare((char *,char *));

/*
  Error declarations.
*/
extern ErrorHandler
  SetErrorHandler _Declare((ErrorHandler)),
  SetWarningHandler _Declare((ErrorHandler));

extern void
  Error _Declare((char *,char *)),
  Warning _Declare((char *,char *));


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


/* -------------------------------------------------------------------------- */

#endif   /* #ifdef IVI_HAVE_X11  */

#endif   /* #ifndef __C2MAN__  */

