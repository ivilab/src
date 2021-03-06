
/* $Id: i_lib.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_LIB_INCLUDED
#define I_LIB_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "l/l_word_list.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int get_image_file_base_path
(
    const char* file_path,
    char*       base_path,
    size_t      base_path_size,
    char*       suffix,
    size_t      suffix_size
);

int get_image_file_base_name
(
    const char* file_name,
    char*       dir_str,
    size_t      dir_str_size,
    char*       base_name,
    size_t      base_name_size,
    char*       suffix,
    size_t      suffix_size
);

int get_image_files
(
    Word_list** paths_ptr_ptr, 
    Word_list** base_names_ptr_ptr,
    const char* dir, 
    const char* suffix
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

