
/* $Id: l_config.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef L_CONFIG_INCLUDED
#define L_CONFIG_INCLUDED


#include "l/l_def.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int   check_config_sub_dir      (const char* sub_dir);

int   read_dbl_from_config_file
(
    double*     value_ptr,
    const char* env_var,
    const char* directory,
    const char* file_name,
    const char* message_name,
    char*       config_file_name,
    size_t      config_file_name_size
);

FILE* open_config_file
(
    const char* env_var,
    const char* sub_dir,
    const char* file_name,
    const char* message_name
);

int   get_config_file
(
    const char* env_var,
    const char* sub_dir,
    const char* file_name,
    const char* message_name,
    char*       config_file,
    size_t      config_file_size
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

