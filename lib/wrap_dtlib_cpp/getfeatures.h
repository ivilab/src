#ifndef _GETFEATURES_H
#define _GETFEATURES_H

/*
 * Kobus: We have run into trouble with 32 bit centric code in this
 * distribution. I have changed some long's to ivi_int32's.  The immediate
 * problem is that the segmentation maps can get written out as 64 bit integers. 
*/
#include "l/l_sys_def.h"
#include "m_cpp/m_matrix.h"

#warning "[Code police] Do not put 'using namespace' in global scope of header."
using namespace ivi_c;

#include "wrap_dtlib_cpp/img.h"
#include "wrap_dtlib_cpp/matlab.h"

namespace DTLib {
     ///////////////////////////////////////////////////////////////////////////
     // added by PINAR

     void GetTextureFeatures(int *SegBuf,
                             int Width,
                             int Height,
                             int nGaussScales,
                             int nGaussOrientations,
                             CImgVec<float>& OEVec,
                             CImgVec<float>& ConvVec,
                             int nDOGScales,
                             ivi::Matrix & Oe_mean,
                             ivi::Matrix & Oe_var,
                             ivi::Matrix & DOG_mean,
                             ivi::Matrix & DOG_var
);


} // namespace DTLib {

#endif /* #ifndef _GETFEATURES_H */
