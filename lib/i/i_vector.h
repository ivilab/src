
/* $Id: i_vector.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2009 by Kobus Barnard.
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
#ifndef I_VECTOR_INCLUDED
#define I_VECTOR_INCLUDED

/* Start for c2man */
#include "i/i_float.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

typedef struct IVI_image_vector
{
  int length;
  struct IVI_image **images;
}
IVI_image_vector;

#ifdef TRACK_MEMORY_ALLOCATION

#   define read_image_vector(w, x, y, z)             \
                         debug_read_image_vector(w, x, y, z, __FILE__,  __LINE__)

  int debug_read_image_vector
  (
      IVI_image_vector **ivpp,
      const char *file_pattern,
      int         num_rows,
      int         num_cols,
      const char* filename,
      int         line_number
  );

#   define copy_image_vector(y, z)                                  \
                         debug_copy_image_vector(y, z, __FILE__,  __LINE__)

  int debug_copy_image_vector
  (
     IVI_image_vector **target_ivpp, 
     const IVI_image_vector *source_ivp,
     const char* filename,
     int         line_number
  );

#   define get_target_image_vector(w, x, y, z)                           \
                                 debug_get_target_image_vector(w, x, y, z, __FILE__,  __LINE__)

  int debug_get_target_image_vector
  (
     IVI_image_vector **target_ivpp,
     int         num_images,
     int         num_rows,
     int         num_cols,
     const char* filename,
     int         line_number
  );
 
#else
  int read_image_vector
  (
     IVI_image_vector **target_ivpp, 
     const char *file_pattern,
     int num_rows, int num_cols
  );
  
  int copy_image_vector
  (
     IVI_image_vector **target_ivpp, 
     const IVI_image_vector *source_ivp
  );
  
  int get_target_image_vector
  (
     IVI_image_vector **target_ivpp, 
     int num_images, 
     int num_rows, 
     int num_cols
  );
#endif
  void free_image_vector
  (
     IVI_image_vector *ivp
  );

#ifdef __cplusplus
}
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
/* End for c2man */

#endif

#endif
