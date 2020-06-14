
/* $Id: i_valid.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef I_VALID_INCLUDED
#define I_VALID_INCLUDED


#include "l/l_def.h"
#include "i/i_type.h"
#include "i/i_float.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_image_validity_options(const char* option, const char* value);

int mark_clipped_pixels(IVI_image* ip);
int mark_dark_pixels(IVI_image* ip);
int unmark_dark_pixels(IVI_image* ip);

int mark_blooming_candidates(IVI_image** out_ipp, const IVI_image* in_ip);

int mark_pixels_above_threshold(IVI_image* ip, double max);
int mark_pixels_below_threshold(IVI_image* ip, double min);
int count_invalid_pixels(const IVI_image* ip);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


