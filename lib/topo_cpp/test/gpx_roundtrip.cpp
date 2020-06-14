/**
 * @file
 * @author Andy Predoehl
 * @brief test file for GPX load then save (GPX round-trip)
 */
/*
 * $Id: gpx_roundtrip.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_incl.h>
#include <l_cpp/l_stdio_wrap.h>
#include <topo_cpp/autolayer.h>

#include <iostream>
#include <vector>

// make up some random UTM coordinates, save to file, read file, compare.
int main2( int, const char* const* )
{
    const std::string filename1 = "points.gpx", filename2 = "schmoints.gpx";

    // load gpx file
    ivi::TopoFusion::AutoLayer l( filename1 );

    // write it
    IVI( ERE( l.write( filename2 ) ) );

    off_t fs1, fs2;
    IVI( ERE( get_file_size( filename1.c_str(), &fs1 ) ) );
    IVI( ERE( get_file_size( filename2.c_str(), &fs2 ) ) );
    if ( fs1 != fs2 )
    {
        ivi_c::set_error( "Files differ in size" );
        return ivi_c::ERROR;
    }

    ivi::File_Ptr_Read f1( filename1 ), f2( filename2 );

    // verify files are identical
    off_t ix = 0;
    for( int c1,c2; (c1 = fgetc( f1 )) != EOF && (c2 = fgetc( f2 )) != EOF; )
    {
        if ( c1 != c2 )
        {
            ivi_c::set_error( "Files differ at offset %u\n", ix );
            return ivi_c::ERROR;
        }
        ++ix;
    }

    IVI( ERE( ivi_unlink( filename2.c_str() ) ) );
    return ivi_c::NO_ERROR;
}

int main( int argc, const char* const* argv )
{
    IVI( EPETE( ivi_init() ) );

    try
    {
        IVI( EPETE( main2( argc, argv ) ) );
    }
    catch( const ivi::IVI_error& e )
    {
        e.print_details_exit();
        /* NOTREACHED */
        exit( 1 );
    }

    if ( ivi_c::is_interactive() )
    {
        ivi_c::pso( "Success!\n" );
    }

    ivi_c::ivi_cleanup();

    return EXIT_SUCCESS;
}
