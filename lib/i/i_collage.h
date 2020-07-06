
/* $Id: i_collage.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_COLLAGE_INCLUDED
#define I_COLLAGE_INCLUDED 

/* Start for c2man */


#include "l/l_def.h"
#include "l/l_int_matrix.h"
#include "m/m_matrix.h"
#include "i/i_type.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

#ifdef HOW_IT_WAS
#define XXX_MAX_MONTAGE_IMAGE_LABEL_SIZE  200
#endif

/* -------------------------------------------------------------------------- */

int set_collage_options(const char* option, const char* value);

int make_image_collage
(
    IVI_image** out_ipp,
    int         num_vertical,
    int         num_horizontal,
    const IVI_image* const* ip_list
);

int make_image_collage_2
(
    IVI_image** out_ipp,
    int         num_vertical,
    int         num_horizontal,
    const IVI_image* const* ip_list,
    Pixel*      background_colour_ptr,
    Pixel*      border_colour_ptr,
    int         outside_border_width,
    int         division_border_width,
    int         horizontal_divisions_arg,
    int         vertical_divisions_arg,
    int         square_images
);

int make_compact_image_collage
(
    IVI_image** out_ipp,
    int         num_vertical,
    int         num_horizontal,
    IVI_image** ip_list
);

int make_compact_image_collage_2
(
    IVI_image**  out_ipp,
    int          num_vertical,
    int          num_horizontal,
    IVI_image**  ip_list,
    Int_matrix** image_coords_mpp
);


#ifdef HOW_IT_WAS
int ip_output_montage
(
    const char* output_file,
    int         num_images,
    int         num_montage_rows,
    int         num_montage_cols,
    IVI_image** images,
    char        labels[][ XXX_MAX_MONTAGE_IMAGE_LABEL_SIZE ],
    const char* extra
);
#endif

int output_montage
(
    const char* output_file,
    int         num_images,
    int         num_montage_rows,
    int         num_montage_cols,
    char        image_names[][ MAX_FILE_NAME_SIZE ],
    char**      labels,
    const char* extra
);

int output_montage_2
(
    const char* output_file,
    const char* geometry_str,
    int         num_images,
    int         num_montage_rows,
    int         num_montage_cols,
    char        image_names[][ MAX_FILE_NAME_SIZE ],
    char**      labels,
    const char* extra
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


