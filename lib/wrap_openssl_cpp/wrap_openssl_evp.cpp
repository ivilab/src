/**
 * @file
 * @author Andrew Predoehl
 * @brief Implemenation of finalize method for wrapper on OpenSSL EVP object
 */
/*
 * $Id: wrap_openssl_evp.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include "l/l_sys_debug.h"  /* For ASSERT */
#include "l/l_sys_lib.h"
#include "l/l_sys_io.h"
#include "l_cpp/l_util.h"
#include <iostream>
#include "wrap_openssl_cpp/wrap_openssl_evp.h"

#ifdef IVI_HAVE_OPENSSL
#include <algorithm>
#endif

namespace ivi
{

void OpenSSL_EVP::finalize()
{
#ifdef IVI_HAVE_OPENSSL
    Unit md[ EVP_MAX_MD_SIZE ];
    unsigned mdsize = 0;
    ETX( 0 == EVP_DigestFinal_ex( &m_context, md, &mdsize ) );
    ASSERT( 0 < mdsize && mdsize <= EVP_MAX_MD_SIZE );
    ASSERT( m_digest.size() == mdsize );
    std::copy( md, md + mdsize, m_digest.begin() );
    m_finalized = true;
#endif
}

} // end namespace ivi

