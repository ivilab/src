/**
 * @file
 * @brief GSL utility stuff to help those using the C++ wrapper on GSL code.
 * @author Andrew Predoehl
 *
 * GSL is the GNU Scientific Library.
 */

/*
 * $Id: gsl_util.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_io.h>
#include <l_cpp/l_exception.h>

#include "gsl_cpp/gsl_util.h"

#include <sstream>

namespace ivi {

void report_gsl_failure_and_throw_ivi_error(
    int gsl_code,
    const char* file,
    unsigned line_no
)
{
#ifdef IVI_HAVE_GSL
    if ( gsl_code != GSL_SUCCESS )
    {
        std::ostringstream oss;
        oss << "GSL Error E" << gsl_code << ", "
                                        << gsl_strerror( gsl_code ) << '\n';
        ivi::throw_ivi_error( oss.str().c_str(), file, line_no );
    }
#endif
}


void gsl_iterate_EPE(int gsl_error_code)
{
#ifdef IVI_HAVE_GSL
    if (gsl_error_code != GSL_SUCCESS)
    {
        ivi_c::p_stderr("iterate failure E%d: %s\n", gsl_error_code,
                                                     gsl_strerror(gsl_error_code));
    }
#endif
}


}
