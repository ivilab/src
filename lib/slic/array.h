/* $Id: array.h 25612 2020-07-06 22:17:23Z kobus $
 */
#ifndef SLIC_INCLUDED_ARRAY_H_
#define SLIC_INCLUDED_ARRAY_H_

/* Start for c2man */



/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#ifdef __C2MAN__
#    include "l/l_def.h"
#endif 


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

