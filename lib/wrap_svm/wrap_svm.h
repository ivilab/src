
/* $Id: wrap_svm.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|                                                                              |
|  Copyright (c) 2005-2008 by members of University of Arizona Computer Vision|
|  group (the authors) including                                               |
|        Ranjini Swaminathan
|        Kobus Barnard.                                                        |
|                                                                              |
|  (Copyright only applies to the wrapping code, not the wrapped code).        |
|                                                                              |
|  Personal and educational use of this code is granted, provided that this    |
|  header is kept intact, and that the authorship is not misrepresented, that  |
|  its use is acknowledged in publications, and relevant papers are cited.      |
|                                                                              |
|  For other use contact the author (kobus AT cs DOT arizona DOT edu).         |
|                                                                              |
|  Please note that the code in this file has not necessarily been adequately  |
|  tested. Naturally, there is no guarantee of performance, support, or fitness|
|  for any particular task. Nonetheless, I am interested in hearing about      |
|  problems that you encounter.                                                |
|                                                                              |
* =========================================================================== */


#ifndef WRAP_SVM_INCLUDED
#define WRAP_SVM_INCLUDED

/* Start for c2man */

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


int set_svm_options (const char* option, const char* value);
int compute_svm     (const char*, const char*, const char*);

int predict_with_svm
(
    const char* ,
    const char* ,
    const char* ,
    const char*
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

