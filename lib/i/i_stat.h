
/* $Id: i_stat.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_STAT_INCLUDED
#define I_STAT_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
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


int get_ave_rgb(Vector**, const IVI_image*);

int get_max_rgb(Vector**, const IVI_image*);

int get_image_stats
(
    int*             num_valid_pixels_ptr,
    Vector**         mean_vpp,
    Vector**         stdev_vpp,
    const IVI_image* source_ip
);

int get_image_window_stats
(
    int*             num_valid_pixels_ptr,
    Vector**         mean_vpp,
    Vector**         stdev_vpp,
    const IVI_image* source_ip,
    int              row_offset,
    int              col_offset,
    int              num_target_rows,
    int              num_target_cols
);

int get_ave_ratio_without_invalid
(
    Vector**         out_vpp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip,
    double           threshold,
    int              min_num_good_points
);

int get_ave_sum_ratio_without_invalid
(
    double*          ratio_ptr,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip,
    double           threshold,
    int              min_num_good_points
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


