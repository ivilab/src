/**
 * @file
 * @brief GSL random number generator code -- serialization code, that is
 * @author Andrew Predoehl
 *
 * GSL is the GNU Scientific Library.
 */

/*
 * $Id: gsl_rng.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_lib.h>
#include <l/l_sys_io.h>
#include <l/l_error.h>
#include <l_cpp/l_exception.h>
#include <l_cpp/l_util.h>

#include "gsl_cpp/gsl_rng.h"

#include <sstream>

namespace {

FILE* make_ephemeral_file()
{
    // Make a temporary file.
    char fname[ 512 ];
    IVI( ETX( BUFF_GET_TEMP_FILE_NAME( fname ) ) );
    FILE* fp = ivi_c::ivi_fopen( fname, "w+" );
    ETX( 00 == fp );

    // This is the ol' unix trick, "unlink filename while file is still open."
    int rc1 = ivi_c::ivi_unlink( fname );
    if ( ivi_c::ERROR == rc1 )
    {
        ETX( ivi_c::ivi_fclose( fp ) );
        IVI_THROW_2( ivi::IO_error,
                                "Failed to unlink serialization temp file" );
    }

    return fp;
}


} // end anonymous ns



namespace ivi {

#ifdef IVI_HAVE_GSL
std::string gsl_rng_serialize_implementation( const gsl_rng* rng )
{
    FILE* fp = make_ephemeral_file();

    // Write the state bytes into the temporary file.
    int rc2 = gsl_rng_fwrite( fp, rng );

    // Try to read it back again into a string (if the write succeeded).
    if ( 0 == rc2 )
    {
        std::string state;
        rewind( fp );
        for( int ccc; ( ccc = ivi_c::ivi_fgetc( fp ) ) != EOF; )
        {
            state.push_back( char( ccc ) );
        }
        ETX( ivi_c::ivi_fclose( fp ) );
        return state;
    }

    // Write has failed us.
    ETX( ivi_c::ivi_fclose( fp ) );
    GSL_ETX( rc2 );

    /* NOTREACHED */
    IVI_THROW_2( IVI_error, "gsl_rng_fwrite() returned malformed error code" );
}


void gsl_rng_deserialize_implementation(
    gsl_rng* rng,
    const std::string& state
)
{
    FILE* fp = make_ephemeral_file();

    // write state into temporary file
    for( size_t iii = 0; iii < state.size(); ++iii )
    {
        int rc1 = ivi_c::ivi_fputc( fp, state[ iii ] );
        if ( EOF == rc1 )
        {
            ETX( ivi_c::ivi_fclose( fp ) );
            IVI_THROW_2( IVI_error, "write failure deserializing GSL RNG" );
        }
    }

    // read state from temporary file
    rewind( fp );
    int rc2 = gsl_rng_fread( fp, rng );
    ETX( ivi_c::ivi_fclose( fp ) );
    GSL_ETX( rc2 );
}
#endif


}
