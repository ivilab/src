
/* $Id: i_bw_byte.h 25612 2020-07-06 22:17:23Z kobus $ */

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


#ifndef IRCAMERA_IO_H_INCLUDED
#define IRCAMERA_IO_H_INCLUDED

/* Start for c2man */

  
#include "i/i_gen.h"  

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

/* =============================================================================
 *                             Bw_byte_image
 *
 * Type for black-and-white byte images
 *
 * This is the black-and-white image type for the IVI library. There is also a
 * color image format (IVI_image) which the basic image format.
 *
 * Index: images
 *
 * Documentor: Ernesto Brau
 *
 * -----------------------------------------------------------------------------
*/

  
typedef struct Bw_byte_image
{
    int          num_rows;
    int          num_cols;
    int          read_only;
    unsigned char** pixels;
}
Bw_byte_image;
  
/* -------------------------------------------------------------------------- */


int get_target_bw_byte_image(Bw_byte_image **target_ipp, int num_rows, int num_cols);

void ivi_free_bw_byte_image(Bw_byte_image* ip);

int rotate_bw_byte_image(Bw_byte_image** target_ipp, const Bw_byte_image* ip); 

int ivi_copy_bw_byte_image(Bw_byte_image** target_ipp, const Bw_byte_image* source_ip);

int bw_byte_image_to_ivi_image(IVI_image ** target_ipp, const Bw_byte_image* source_ip);

int get_bw_byte_image_face_region(Bw_byte_image **target_ipp, Bw_byte_image * source_ip, int x, int y, int width, int height);

int ivi_image_to_bw_byte_image ( Bw_byte_image ** bw_image, const IVI_image * ivi_image );

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

