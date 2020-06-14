/**
 * @file
 * @author Andy Predoehl
 * @brief test file for GPX save and load
 */
/*
 * $Id: test_write.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_incl.h>
#include <l_cpp/l_stdio_wrap.h>
#include <topo_cpp/autolayer.h>
#include <topo_cpp/xml.h>

#include <iostream>
#include <vector>

// make up some random UTM coordinates, save to file, read file, compare.
int main2()
{
    // make up random coords
    ivi::Temporary_File tf;
    ivi::TopoFusion::AutoLayer l;
    std::vector< ivi::TopoFusion::pt > p( 10 );

    for( size_t iii = 0; iii < p.size(); ++iii )
    {
        p.at( iii ) = ivi::TopoFusion::make_pt( (25+ivi_c::ivi_rand())*10000,
                                    ivi_c::ivi_rand()*100000 + 3000000, 12);
    }
    l.set_track0points( p.size(), & p.front() );

    // save to file
    int rc = l.write( tf.get_filename() );
    if ( rc != ivi_c::NO_ERROR ) return EXIT_FAILURE;

    // read file
    ivi::TopoFusion::AutoLayer m( tf.get_filename() );

    // compare
    for( size_t iii = 0; iii < p.size(); ++iii )
    {
        using namespace ivi_c;
        const ivi::TopoFusion::layer &q = m;
        ASSERT( fabs( p[iii].x - q.tracks[0].s.points[iii].x ) < 0.1 );
        ASSERT( fabs( p[iii].y - q.tracks[0].s.points[iii].y ) < 0.1 );
        ASSERT( p[iii].zone == q.tracks[0].s.points[iii].zone );
    }

#if 0 /* print it all out, if the above assertions are failing */
    for( size_t iii = 0; iii < p.size(); ++iii )
    {
        const ivi::TopoFusion::layer &q = m;
        std::cerr   << p[iii].x << '\t'
                    << p[iii].y << '\t'
                    << int( p[iii].zone ) << '\n';
    }
    std::cerr << '\n';
    for( size_t iii = 0; iii < p.size(); ++iii )
    {
        const ivi::TopoFusion::layer &q = m;
        std::cerr   << q.tracks[0].s.points[iii].x << '\t'
                    << q.tracks[0].s.points[iii].y << '\t'
                    << int( q.tracks[0].s.points[iii].zone ) << '\n';
    }
    std::cerr << '\n';
    for( size_t iii = 0; iii < p.size(); ++iii )
    {
        const ivi::TopoFusion::layer &q = m;
        std::cerr   << p[iii].x - q.tracks[0].s.points[iii].x << '\t'
                    << p[iii].y - q.tracks[0].s.points[iii].y << '\t'
                    << int( p[iii].zone - q.tracks[0].s.points[iii].zone )
                    << '\n';
    }
#endif

    return EXIT_SUCCESS;
}

int main()
{
    ivi_c::ivi_init();

    int rc;
    try
    {
        rc = main2();
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
    return rc;
}
