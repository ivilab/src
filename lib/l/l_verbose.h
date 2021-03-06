
/* $Id: l_verbose.h 25866 2020-10-19 15:15:55Z kobus $ */

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
* =========================================================================== */

#ifndef L_VERBOSE_INCLUDED
#define L_VERBOSE_INCLUDED

/* Start for c2man */


#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_verbose_options  (const char* option, const char* value);
int ivi_set_verbose_level(int new_level);
int ivi_get_verbose_level(void);

#ifndef SGI /* SGI pretends it is lint and gives spurious messages. */
/*PRINTFLIKE2*/
#endif
long verbose_pso(int, const char*, ...);

long verbose_puts(int cut_off, const char* buff);

#ifndef SGI /* SGI pretends it is lint and gives spurious messages. */
/*PRINTFLIKE1*/
#endif

long warn_pso(const char* format_str, ...);

#ifndef SGI /* SGI pretends it is lint and gives spurious messages. */
/*PRINTFLIKE1*/
#endif
long warn_pse(const char* format_str, ...);

long warn_fputs(FILE* fp, const char* buff);

/* =============================================================================
 *                             warn_puts
 *
 * (MACRO) Printing to stdout with prefix "<< Warning >> "
 *
 * This macro is defined as warn_fputs(stdout, (x)). 
 *
 * This macro writes a string to stdout with the prefix "<< Warning >> "
 * pre-pended to each line. therwise it is similar to ivi_puts.
 *
 * Warnings can be suppressed by the user with "set warnings = off" or by the
 * programmer by setting up the approprate call to the option infrastructure.
 *
 * Returns:
 *    Returns the number of characters written.  ERROR is returned if there is
 *    an error.
 *
 * Related:
 *    warn_pso, warn_fputs
 *
 * Index: I/O, warning messages, debugging
 *
 * -----------------------------------------------------------------------------
*/

#ifdef __C2MAN__    /* Pretend we are a ".c" file to document MACROS. */
    long warn_puts(const char* output_string);
#endif

#ifndef __C2MAN__  /* Just doing "else" confuses ctags for the first such line.
                      I have no idea what the problem is. */
#   define warn_puts(x)                warn_fputs(stdout, (x))
#endif

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifndef SGI /* SGI pretends it is lint and gives spurious messages. */
/*PRINTFLIKE1*/
#endif
long interactive_pso(const char*, ...);

long interactive_puts(const char* buff);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

