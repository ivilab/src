
/* $Id: t3_segment.h 25499 2020-06-14 13:26:04Z kobus $ */

/*
    Copyright (c) 1994-2008 by Kobus Barnard (author).

    Personal and educational use of this code is granted, provided
    that this header is kept intact, and that the authorship is not
    misrepresented. Commercial use is not permitted.
*/

#ifndef T3_SEGMENT_INCLUDED
#define T3_SEGMENT_INCLUDED


#include "l/l_def.h"
#include "i/i_gen.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

/* =============================================================================
 *                             Segment_t3
 *
 * Type for segmentation results
 *
 * This type was introduced to be part of the Segmentation_t3 type.
 *
 * Index: images
 *
 * -----------------------------------------------------------------------------
*/

/* Changes here must be reflected in the copy routine, etc. */

typedef struct Segment_t3
{
    int         segment_number;
    int         num_pixels;
    int         num_boundary_pixels;
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
    int         num_neighbours;
    int*        neighbours;
    double*     nb_edge_r_diffs;
    double*     nb_edge_g_diffs;
    double*     nb_edge_rel_sum_diffs;
    int*        connection_counts;
    int         pixel_count;          /* Private, should be same as num_pixels. */
    int         boundary_pixel_count; /* Private */
}
Segment_t3;


/* =============================================================================
 *                             Segmentation_t3
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
typedef struct Segmentation_t3
{
    int          num_rows;
    int          num_cols;
    int          num_segments;
    Segment_t3** segments;
    int          negative_num_segments;
    Segment_t3** negative_segments;
    int**        seg_map;
}
Segmentation_t3;

/* -------------------------------------------------------------------------- */

int  t3_set_segmentation_options(const char* option, const char* value);

int  t3_segment_image
(
    const IVI_image*  ip,
    const Int_matrix* initial_region_map,
    Segmentation_t3** segmentation_ptr_ptr
);

int  t3_resegment_image
(
    const IVI_image*  ip,
    const Int_matrix* region_map_mp,
    Segmentation_t3** segmentation_ptr_ptr
);

int  t3_copy_segmentation
(
    Segmentation_t3**      segmentation_ptr_ptr,
    const Segmentation_t3* segmentation_ptr
);

void t3_free_segmentation       (Segmentation_t3* segmentation_ptr);

#ifdef OBSOLETE
int  t3_image_draw_segmentation
(
    const IVI_image* ip,
    Segmentation_t3* segmentation_ptr,
    IVI_image**      out_ip_list
);

int  t3_image_draw_segments_and_outside_boundaries
(
    const IVI_image* ip,
    Segmentation_t3* segmentation_ptr,
    IVI_image**      out_ipp
);
#endif

int  t3_image_draw_image_segments
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr
);

int  t3_image_draw_image_segment_outside_boundaries
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr
);

int  t3_image_draw_image_segment_outside_boundaries_2
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr
);

int  t3_image_draw_image_segment_boundaries
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr,
    int                    width
);

int t3_image_draw_image_segment_fancy_boundaries
(
    IVI_image*          ip,
    const Segmentation_t3* segmentation_ptr,
    int                 width
);

int  t3_image_draw_image_segment_boundaries_2
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr,
    int                    width
);

int  t3_image_draw_non_segments
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr,
    int                    R,
    int                    G,
    int                    B
);

int  t3_image_draw_image_segment_neighbours
(
    IVI_image*             ip,
    const Segmentation_t3* segmentation_ptr,
    int                    width,
    int                    R,
    int                    G,
    int                    B
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif


