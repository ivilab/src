
/* $Id: t1_segment.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef T1_SEGMENT_INCLUDED
#define T1_SEGMENT_INCLUDED

/* Start for c2man */


#include "l/l_def.h"
#include "i/i_gen.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

typedef struct Segment_t1
{
    int         segment_number;
    int         num_pixels;
    int         num_boundary_pixels;
    int         num_neighbours;
    double      R_ave;
    double      G_ave;
    double      B_ave;
    double      r_chrom_ave;
    double      g_chrom_ave;
    double      sum_RGB_ave;
    double      i_CM;
    double      j_CM;
    int         i_inside;
    int         j_inside;
    double      first_moment;
    double      second_moment;
    int         i_min;
    int         i_max;
    int         j_min;
    int         j_max;
    double      boundary_len;
    double      outside_boundary_len;
    Pixel_info* pixels;
    Pixel_info* boundary_pixels;
    Matrix*     outside_boundary_mp;
    int*        neighbours;
}
Segment_t1;


/* =============================================================================
 *                             Segmentation_t1
 *
 * Type for segmentation results
 *
 * This type is a representation of a segmented image.
 *
 * Index: images
 *
 * -----------------------------------------------------------------------------
*/

/* Changes here must be reflected in the copy routine, etc. */
typedef struct Segmentation_t1
{
    int          num_rows;
    int          num_cols;
    int          num_segments;
    Segment_t1** segments;
    int          negative_num_segments;
    Segment_t1** negative_segments;
    int**        seg_map;
}
Segmentation_t1;

/* -------------------------------------------------------------------------- */

int  t1_set_segmentation_options(const char* option, const char* value);

int  t1_segment_image
(
    const IVI_image*  ip,
    Segmentation_t1** segmentation_ptr_ptr
);

void t1_free_segmentation       (Segmentation_t1* segmentation_ptr);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


