/**
 * @file
 * @brief unit test for html hex triplet output of pixel channel values
 * @author Andrew Predoehl
 */
/*
 * $Id: test_html.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_def.h>
#include <l/l_sys_lib.h>
#include <i_cpp/i_pixel.h>

#include <iostream>
#include <cassert>

int main()
{
    ivi::PixelRGBA p1( 255, 0, 0 ); // red
    assert( 0 == strcmp( p1.as_hex_triplet().c_str(), "#ff0000" ) );

    ivi::PixelRGBA p2( 0, 255, 0 ); // green
    assert( 0 == strcmp( p2.as_hex_triplet().c_str(), "#00ff00" ) );

    ivi::PixelRGBA p3( 0, 0, 255 ); // blue
    assert( 0 == strcmp( p3.as_hex_triplet().c_str(), "#0000ff" ) );

    ivi::PixelRGBA p4( 0, 0, 0 );   // black
    assert( 0 == strcmp( p4.as_hex_triplet().c_str(), "#000000" ) );

    ivi::PixelRGBA p5( 255, 255, 255 ); // white
    assert( 0 == strcmp( p5.as_hex_triplet().c_str(), "#ffffff" ) );

    if ( ivi_c::is_interactive() )
    {
        std::cout << __FILE__ << ": success!\n";
    }

    return EXIT_SUCCESS;
}
