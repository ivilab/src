
/* $Id: i_seq.h 25612 2020-07-06 22:17:23Z kobus $ */

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

#ifndef I_SEQ_INCLUDED
#define I_SEQ_INCLUDED

/* Start for c2man */

#include "l/l_incl.h"
#include "m/m_matrix.h"
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

/* -------------------------------------------------------------------------- */


/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/* =============================================================================
 *                                   IVI_image_sequence
 *
 * Sequence of images type
 *
 * A type which implements a sequence of images
 *
 * Index: images
 *
 * -----------------------------------------------------------------------------
 */

typedef struct IVI_image_sequence
{
    int length;
    IVI_image** elements;
}
IVI_image_sequence;

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

int get_target_image_sequence(IVI_image_sequence** target_ispp, int length);

int read_image_sequence(IVI_image_sequence** target_ispp, const Word_list* filenames);

void free_image_sequence(IVI_image_sequence* isp);

int average_bw_images
(
    IVI_image**               avg_img,
    const IVI_image_sequence* images
);

int std_dev_bw_images
(
    IVI_image**               std_dev_img,
    const IVI_image_sequence* images,
    const IVI_image*          avg_img
);

int average_bw_images_2
(
    Matrix**                  avg_img_mat,
    const IVI_image_sequence* images
);

int std_dev_bw_images_2
(
    Matrix**                  std_dev_img_mat,
    const IVI_image_sequence* images,
    const Matrix*             avg_img_mat
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif


