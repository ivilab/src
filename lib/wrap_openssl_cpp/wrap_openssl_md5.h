/**
 * @file
 * @author Andrew Predoehl
 * @brief Wrapper for MD5 (message digest 5) hash provided by OpenSSL
 *
 * Wrapped so it will still compile even with NO_LIBS.
 *
 * Please see class ivi::MD5.
 */
/*
 * $Id: wrap_openssl_md5.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef OPENSSL_CPP_MD5_H_WRAP_PREDOEHL_UOFARIZONA_VISION
#define OPENSSL_CPP_MD5_H_WRAP_PREDOEHL_UOFARIZONA_VISION

#include <wrap_openssl_cpp/wrap_openssl_evp.h>

namespace ivi
{

/**
 * @brief Message Digest 5 computation, using OpenSSL code
 *
 * @throws ivi::Missing_dependency if compiled with NO_LIBS or missing OpenSSL
 */
class MD5
:   public OpenSSL_EVP
{
public:
#ifdef IVI_HAVE_OPENSSL
    MD5()
    :   OpenSSL_EVP( EVP_md5() )
    {}
#endif
};


} // end namespace ivi

#endif

