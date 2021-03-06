#ifndef SLIC_BACKPROJECT_INCLUDED
#define SLIC_BACKPROJECT_INCLUDED


#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

/* Kobus. Dot H files should be stand alone. */

#include "i/i_incl.h" 

/* End of includes for c2man */

int back_project_frame_slide_to_slide
 (
  const IVI_image *frame_ip,
  int slide_width,
  int slide_height,
  const Matrix    *H_mp,
  int             fitting_model,
  IVI_image       **bp_ipp,
  Int_matrix      **mask_impp
   );

/* int back_project_frame_slide_to_slide_enlarged */
/*  ( */
/*   const IVI_image *frame_ip, */
/*   int slide_width, */
/*   int slide_height, */
/*   int enlarged_slide_width, */
/*   int enlarged_slide_height, */
/*   const Matrix    *H_mp, */
/*   int             fitting_model, */
/*   IVI_image       **bp_ipp, */
/*   Int_matrix      **mask_impp */
/*    ); */


int back_project_frame_slide_to_slide_enlarged
 (
  const IVI_image *frame_ip,
  int slide_width,
  int slide_height,
  int enlarged_slide_width,
  int enlarged_slide_height,
  int frame_width, 
  int frame_height,
  const Matrix    *H_mp,
  int             fitting_model,
  IVI_image       **bp_ipp,
  Int_matrix      **mask_impp
   );

/* 
 * Kobus: 2019/09/06.
 * This code is more or less duplicated in lib/i_geometric/i_geometric_maping.c.
 * It is not clear which is newer. Both could use rationalization. My guess is
 * that we have an unfinished attempt to put slic code that is not secficic to
 * slic elsewhere. For now, I have renamed the version in
 * lib/i_geometric/i_geometric_maping.c to "back_project_image_new". 
*/
int back_project_image
(
 const IVI_image *ip,
 const Matrix    *H_mp,
 int             fitting_model,
 IVI_image       **bp_ipp,
 Int_matrix      **mask_impp
 );


int get_enlarged_frame_dimensions
  (
   const Matrix *slide_to_frame_mp,
   int slide_width,
   int slide_height,
   int frame_width,
   int frame_height,
   Int_matrix **enlarged_frame_dimensions
  );

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
