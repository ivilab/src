
/* $Id: i2_collage.h 25499 2020-06-14 13:26:04Z kobus $ */

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

#ifndef I2_COLLAGE_INCLUDED
#define I2_COLLAGE_INCLUDED


#include "l/l_def.h"
#include "m/m_matrix.h"
#include "i/i_type.h"
#include "i/i_float.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */


#define MAX_MONTAGE_IMAGE_LABEL_SIZE  500
#define MAX_NUM_MONTAGE_IMAGES        5000

int ip_output_montage
(
    const char* output_file,
    int         num_images,
    int         num_montage_rows,
    int         num_montage_cols,
    IVI_image** images,
    char        (*labels)[ MAX_MONTAGE_IMAGE_LABEL_SIZE ],
    const char* extra
);

int make_image_collage_with_labels
(
    IVI_image** out_ipp,
    int         num_horizontal,
    int         num_vertical,
    IVI_image** ip_list,
    char**      labels
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


