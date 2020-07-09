/* $Id: array.h 25615 2020-07-09 19:12:05Z kobus $
 */
#ifndef SLIC_INCLUDED_ARRAY_H_
#define SLIC_INCLUDED_ARRAY_H_

/* Start for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#include "l/l_def.h"


/* End of includes for c2man */


typedef struct Array 
{
    void **elements;
    int  length;
}
Array; 



Array *create_array
(
    int length
);

void free_array
(
    Array  *array_ptr,
    void   (*free_element_fn)(void*)
);

void free_array_array
(
    Array  *array_array_ptr,
    void   (*free_array_fn)(void*)
);

int search_array
(
    Array            *array_ptr,
    int              (*key_comp_fn) (const void*, const void*),
    const void       *key_ptr
);

int merge_arrays
(
    const Array *a_ptr,
    const Array *b_ptr,
    Array       **c_ptr_ptr
);

void print_array
(
    const Array * array_ptr, 
    void (*print_fn) (const void *)
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif


/* End for c2man */

#endif

