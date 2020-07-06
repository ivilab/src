
/* $Id: seg_connected_components.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2003-2008 by members of University of Arizona Computer Vision     |
|  group (the authors) including                                               ||
|        Kobus Barnard.                                                        |
|        Prasad Gabbur.                                                        |
|  For use outside the University of Arizona Computer Vision group please      |
|  contact Kobus Barnard.                                                      |
|                                                                              |
* =========================================================================== */


#ifndef CONNECTED_COMPONENTS_MODULE_INCLUDED
#define CONNECTED_COMPONENTS_MODULE_INCLUDED

/* Start for c2man */


/*
// Exported interfaces go here.
*/

#include "m/m_incl.h"
#include "n/n_incl.h"
#include "t3/t3_incl.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

    
typedef struct IVI_region
{
    long               label;
    long               root_label;
    long               moment_00;
    long               moment_10;
    long               moment_01;
    long               min_row;
    long               max_row;
    long               min_col;
    long               max_col;
    struct IVI_region* next_region_ptr;
}
IVI_region;

typedef struct IVI_region_list
{
    int         num_regions;
    IVI_region* regions;
}
IVI_region_list;

int  get_target_region_list(IVI_region_list** rlpp, int num_regions);
void free_region_list      (IVI_region_list* rlp);

int label_four_connected_regions 
(
    Int_matrix**      out_mpp,
    IVI_region_list** region_list_rlpp,
    Segmentation_t3** segmentation_ptr_ptr,
    int*              num_regions_intp,
    int*              max_label_intp,
    const IVI_image*  in_ip,
    const Int_matrix* in_mp,
    const int         merge_eqvlnt_labels 
);

int label_eight_connected_regions
(
    Int_matrix**      out_mpp,
    IVI_region_list** region_list_rlpp,
    Segmentation_t3** segmentation_ptr_ptr,
    int*              num_regions_intp,
    int*              max_label_intp,
    const IVI_image*  in_ip,
    const Int_matrix* in_mp,
    const int         merge_eqvlnt_labels 
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif    /*   #include this file            */


