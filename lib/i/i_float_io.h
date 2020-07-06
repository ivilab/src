
/* $Id: i_float_io.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_FLOAT_IO_INCLUDED
#define I_FLOAT_IO_INCLUDED

/* Start for c2man */


#include "m/m_gen.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_image_input_options(const char* option, const char* value);

int set_image_output_options(const char* option, const char* value);

int ivi_read_image(IVI_image** ip, const char* file_name);

int read_image_from_kiff
(
    IVI_image** ipp,
    FILE*       fp,
    int*        validity_data_possible_ptr
);

int ivi_read_image_2
(
    IVI_image** ipp,
    const char* file_name_gz_and_sub_image
);

int ivi_write_image(const IVI_image* ip, const char* file_name);

#if 0
int write_image_with_transparency(const IVI_image* ip, const char* file_name);
#endif

int display_matrix(const Matrix* mp, const char* title); 

int ivi_display_image(const IVI_image* ip, const char* title);

int fork_display_image(const IVI_image* ip, const char* title);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

