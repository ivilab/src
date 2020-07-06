/* $Id: image_interp.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_IMAGE_INTERP_INCLUDED
#define SLIC_IMAGE_INTERP_INCLUDED

/* Start for c2man */

#include "slic/basic.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif



int transform_image
(
    const IVI_image *src_ip,
    const Matrix    *t_mp,
    int             width,
    int             height,
    const Matrix     *trans_rect_mp,
    int             inter_method,
    IVI_image       **target_ipp,
    Int_matrix      **map_impp
);
int transform_image_w_bounding_box
(
    const IVI_image *src_ip,
    const Matrix    *t_mp,
    int             width,
    int             height,
    int             *slide_coords,
    const Matrix     *trans_rect_mp,
    int             inter_method,
    IVI_image       **target_ipp,
    Int_matrix      **mask_impp
);


int bilinear_inter_image
(
    const IVI_image *src_ip,
    const Matrix     *trans_mp,
    int             width,
    int             height,
    const Matrix     *trans_rect_mp,
    IVI_image        **target_ipp,
    Int_matrix      **map_impp
);

int bilinear_inter_image_w_bounding_box
(
    const IVI_image *src_ip,
    const Matrix     *trans_mp,
    int              width,
    int              height,
    int              *slide_coords,
    const Matrix     *trans_rect_mp,
    IVI_image        **target_ipp,
    Int_matrix       **mask_impp
);

int interp
(
    const IVI_image *src_ip,
    const Matrix     *trans_mp,
    int             width,
    int             height,
    const Matrix     *trans_rect_mp,
    IVI_image        **target_ipp,
    Int_matrix      **map_impp,
    const Matrix    *t_mp
);

/*int get_transformed_bound
(
    const Matrix *a_mp,
    int (*trans_func)(const Matrix *, const Matrix *, Matrix **),
    const Matrix *orig_mp,
    int          bound_width,
    int          bound_height,
    Matrix       **dim_mpp
);
int get_bound
(
    const Matrix *slide_pos_mp,
    int    width,
    int    height,
    Matrix **bound_mpp
);*/

int get_slide_bound
(
    const Matrix *slide_pos_mp,
    int    width,
    int    height,
    Matrix **bound_mpp
);


int get_bound
(
    const Matrix *a_mp,
    int (*trans_func)(const Matrix *, const Matrix *, Matrix **),
    int          top_left_x,
    int          top_left_y,
    int          width,
    int          height,
    Matrix       **dim_mpp
);

int get_transformation_from_bound
(
    Matrix **a_mpp,
    const Matrix *bound_mp,
    int (*fit_func)(const Matrix *, const Matrix *, Matrix **, double*),
    int         top_left_x,
    int         top_left_y,
    int          width,
    int          height
);

int combine_image
(
    IVI_image *img1, 
    IVI_image *img2,
    Matrix *x_mp,
    Matrix *y_mp,
    IVI_image **combined_img
);

int ow_merge_images
(
    IVI_image  *target_ip,
    IVI_image  *src_ip,
    Int_matrix *mask_imp
  );
 
int ow_overlap_images
(
    IVI_image       *target_ip,
    IVI_image       *src_ip,
    Int_matrix      *mask_imp
);

void draw_slide_region
(
    IVI_image *ip,
    Matrix    *slide_pos_mp,
    int r,
    int g,
    int b
);

int SM_subtract_images
(
    IVI_image**      out_ipp,
    const IVI_image* in1_ip,
    const IVI_image* in2_ip,
    const Int_matrix *mask_imp
);
int ow_SM_subtract_images
(
    IVI_image*        in1_ip, 
    const IVI_image*  in2_ip, 
    const Int_matrix* mask_imp
);

int SM_get_images_difference
(
    double           *diff_ptr,
    const IVI_image  *in1_ip,
    const IVI_image  *in2_ip,
    const Int_matrix *mask_imp
);

int SM_get_images_difference_1
(
    double           *diff_ptr,
    const IVI_image  *in1_ip,
    const IVI_image  *in2_ip,
    const Matrix     *slide_pos_mp,
    const Int_matrix *mask_imp
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
