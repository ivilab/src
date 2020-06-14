
/* $Id: mm_io.h 25499 2020-06-14 13:26:04Z kobus $ */

/* =========================================================================== *
|
| Copyright (c) 2000-2008 by Kobus Barnard (author), UCB, and UA. Currently,
| the use of this code is retricted to the UA and UCB image and text projects.
|
* =========================================================================== */

#ifndef MM_IO_INCLUDED
#define MM_IO_INCLUDED


#include "mm/mm_type.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_mmm_io_options(const char* option, const char* value);

int read_multi_modal_model(Multi_modal_model** model_pp, const char* model_dir);

int output_multi_modal_model
(
    const Multi_modal_model* model_ptr,
    int                      num_points,
    const char*              output_dir
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

