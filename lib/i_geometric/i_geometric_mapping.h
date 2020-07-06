
#ifndef I_GEOMETRIC_MAPPING_INCLUDED
#define I_GEOMETRIC_MAPPING_INCLUDED

/* Start for c2man */

#include <i/i_incl.h>


/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

/* 
 * Kobus: 2019/09/06.
 * This code is more or less duplicated in lib/slic. It is not clear which is
 * newer. Both could use rationalization. My guess is that we have an unfinished
 * attempt to put slic code that is not secficic to slic elsewhere. Renaming
 * this version of "back_project_image" to "back_project_image_new". 
*/

int back_project_image_new
(
 const IVI_image *ip,
 const Matrix    *H_mp,
 int             fitting_model,
 IVI_image       **bp_ipp,
 Int_matrix      **mask_impp
 );

int transform_image_homography
(
    const IVI_image *src_ip,
    const Matrix    *t_mp,
    int             width,
    int             height,
    const Matrix     *trans_rect_mp,
    int             inter_method,
    IVI_image       **target_ipp,
    Int_matrix      **mask_impp
);

int bilinear_interpolation
(
    const IVI_image *src_ip,
    const Matrix     *trans_mp,
    int              width,
    int              height,
    const Matrix     *trans_rect_mp,
    IVI_image        **target_ipp,
    Int_matrix       **mask_impp
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif


/* End for c2man */

#endif

