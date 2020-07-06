
/* $Id: monitor.h 25612 2020-07-06 22:17:23Z kobus $ */

#ifndef __C2MAN__     

/*
 * Make sure that these files are not a liability when there is no X11. If this
 * is the case, then comment out the whole thing.
*/
#ifdef IVI_HAVE_X11 

/* -------------------------------------------------------------------------- */


#ifndef MAGICK_MONITOR_INCLUDED   /* Kobus */
#define MAGICK_MONITOR_INCLUDED   /* Kobus */

/* Start for c2man */

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/*
  Monitor typedef declarations.
*/
typedef void
  (*MonitorHandler) _Declare((char *,unsigned int,unsigned int));

/*
  Monitor declarations.
*/
extern MonitorHandler
  SetMonitorHandler _Declare((MonitorHandler));

extern void
  ProgressMonitor _Declare((char *,unsigned int,unsigned int));


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


/* -------------------------------------------------------------------------- */

#endif   /* #ifdef IVI_HAVE_X11  */

/* End for c2man */

#endif   /* #ifndef __C2MAN__  */

