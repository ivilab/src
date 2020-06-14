/**
 * @file
 * @brief Regression tests for the PixPoint and PixPath classes
 * @author Andrew Predoehl
 *
 * This file contains some regression tests for the PixPoint and PixPath class.
 * If any test fails, an assertion will break.  Otherwise the fun will print
 * "success!" and return EXIT_SUCCESS on exit -- nothing fancy.
 */

/*
 * $Id: test_doublecircle.cpp 25499 2020-06-14 13:26:04Z kobus $
 */


#include <l/l_sys_lib.h>
#include <l/l_sys_rand.h>
#include <l/l_sys_io.h>
#include <l/l_debug.h>
#include <l/l_error.h>
#include <l/l_global.h>
#include <l/l_init.h>
#include <l_cpp/l_util.h>
#include <l_cpp/l_test.h>
#include <qd_cpp/pixpoint.h>
#include <qd_cpp/doublecircle.h>


#ifndef SHOW_PRETTY_PICTURES
#define SHOW_PRETTY_PICTURES 0
#endif

#if SHOW_PRETTY_PICTURES
    #include <i_cpp/i_image.h>
    #include <i_cpp/i_pixel.h>
#endif

#include <vector>
#include <iostream>

namespace
{

using ivi::Vector2;
using ivi::qd::DoubleCircle;

#if SHOW_PRETTY_PICTURES

const int EDGE_SZ = 500;

const ivi::PixelRGBA    red     ( 100,   0,   0 ),
                        green   (   0, 100,   0 ),
                        blue    (   0,   0, 100 ),
                        gray    ( 100, 100, 100 ),
                        white   ( 200, 200, 200 );




#endif



/* Test DoubleCircle methods:
 *  ctors
 */
int test12()
{
    using ivi_c::ivi_rand;

    const bool VERBOSE = false; //ivi_c::is_interactive();

    // floating-point equality here is dangerous but seems to be working
    const Vector2 a( 1, 0 ), b( 0, 1 );
    const DoubleCircle circle1( a, b, Vector2( 0, -1 ) );
    TEST_TRUE( 1 == circle1.radius );
    TEST_TRUE( circle1.center == Vector2( 0, 0 ) );

    const DoubleCircle circle2( a, b, Vector2( 2, 1 ) );
    TEST_TRUE( 1 == circle2.radius );
    TEST_TRUE( circle2.center == Vector2( 1, 1 ) );

    ivi_rand();
    ivi_rand();
    ivi_rand();
    ivi_rand();
    ivi_rand();

    for( int iii = 0; iii < 1000; ++iii ) {
        const size_t POPSZ = 20;
        std::vector< Vector2 > vdp;
        Vector2 center(0,0);
        center.x() = 20.0 * ivi_rand() - 10;
        center.y() = 20.0 * ivi_rand() - 10;
        double radius = 10.0 * ivi_rand();
        for( size_t jjj = 0; jjj < POPSZ; ++jjj ) {
            Vector2 ppp(0,0);
            do
            {
                ppp.x() = center.x() + ( 2 * ivi_rand() - 1 ) * radius;
                ppp.y() = center.y() + ( 2 * ivi_rand() - 1 ) * radius;
            }
            while( radius < ( ppp - center ).magnitude() );
            vdp.push_back( ppp );
        }
        TEST_TRUE( POPSZ == vdp.size() );
        DoubleCircle circle3( vdp );
        TEST_TRUE( circle3.radius <= radius );

        if ( VERBOSE )
            std::cout << "Generator center: (" << center
                << "), radius = " << radius
                << "\nFit Circle center: (" << circle3.center
                << "), radius = " << circle3.radius
                << "\nPoints:\nIndex\tPoint\t\t\tRadius\n";

        const DoubleCircle circle3plus(
                circle3.radius * 1.000001,
                circle3.center
            );

        for( size_t jjj = 0; jjj < POPSZ; ++jjj ) {
            if ( VERBOSE )
                std::cout << jjj << '\t' << vdp.at( jjj ) << '\t'
                    << ( vdp.at( jjj ) - circle3.center ).magnitude() << '\n';
            TEST_TRUE( ! circle3plus.outside_me_is( vdp.at( jjj ) ) );
        }
    }

    return EXIT_SUCCESS;
}




/*
 * graphical demonstration of DoubleCircle using the smallest enclosing
 * disc ctor.  This requires macro SHOW_PRETTY_PICTURES to be defined above.
 * Also it requires an "interactive" run on an X client.
 */
int test14()
{
    if ( ! ivi_c::is_interactive() )
    {
        return EXIT_SUCCESS;
    }

#if SHOW_PRETTY_PICTURES
    pid_t p = ivi_c::ivi_fork();
    if (p) return EXIT_SUCCESS;

    // Child creates display, starts an infinite loop.
    ivi::Image img = ivi::Image::create_zero_image( EDGE_SZ, EDGE_SZ );
    std::vector< Vector2 > vp;
    const Vector2   c1( 125, 125 ),
                    c2( c1 + Vector2( 250, 0 ) ),
                    c3( c1 + Vector2( 0, 250 ) ),
                    c4( c1 + Vector2( 250, 250 ) );
    const Vector2* cc[] = { &c1, &c2, &c3, &c4, 00 };
    const ivi::PixelRGBA* pal[] = { &red, &green, &blue, &white, 00 };
    for( const Vector2* const* qq = cc; *qq; ++qq )
    {
        const Vector2& center = **qq;
        const int NCT = 100;
        vp.assign( NCT, Vector2( 0, 0 ) );
        const double    rsx( ivi_c::gauss_rand() * 2 ),
                        rsy( ivi_c::gauss_rand() * 2 ),
                        radiusx( rsx * rsx + 12 ),
                        radiusy( rsy * rsy + 12 );
        for( int iii = 0; iii < NCT; ++iii )
        {
            vp[ iii ].x() = center.x() + ivi_c::gauss_rand() * radiusx;
            vp[ iii ].y() = center.y() + ivi_c::gauss_rand() * radiusy;
        }
        DoubleCircle circle( vp );
        img.draw_circle( circle.center.x(), circle.center.y(), circle.radius, 1,
                                                                        gray );
        for( int iii = 0; iii < NCT; ++iii ) {
            int xxx = vp[ iii ].x(), yyy = vp[ iii ].y();
            TEST_TRUE( 0 <= xxx && xxx < EDGE_SZ );
            TEST_TRUE( 0 <= yyy && yyy < EDGE_SZ );
            ivi_c::Pixel pix = * pal[ qq - cc ];
            img.at( vp[ iii ].x(), vp[ iii ].y() ) = pix;
        }
    }
    img.display( "fit a smallest enclosing disc" );
    while(1) ivi_c::nap(1000);
#endif

    return EXIT_SUCCESS;
}






/// @brief launcher for our PixPath unit tests
int main2( int, const char* const* )
{
    typedef int (*PTest)(void);

    PTest suite[] = { test12, test14, 00 };

    srand( 7654321 );
    for( PTest* p = suite; *p; ++p )
    {
        /*std::cerr << "test sequence " << p - suite << '\n';*/
        int rc = (*p)();
        if ( rc != EXIT_SUCCESS )
        {
            ivi_c::p_stderr( "Failure in test index %d.\n", p - suite );
            return rc;
        }
    }

    if ( ivi_c::is_interactive() )
    {
        ivi_c::pso( "Success!\n" );
    }

    RETURN_VICTORIOUSLY();
}

} // anonymous namespace

int main( int argc, const char* const* argv )
{
    IVI(EPETE(ivi_init()));
    int rc = main2( argc, argv );
    ivi_c::ivi_cleanup();
    return rc;
}

