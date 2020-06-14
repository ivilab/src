#include <l/l_sys_lib.h>
#include <l/l_error.h>
#include <l/l_global.h>
#include <l/l_init.h>
#include <l_cpp/l_util.h>
#include <l_cpp/l_stdio_wrap.h>

#include <vector>
#include <cstdio>
#include <cassert>

namespace {

const std::string   text1 = "Once upon a time,\n",
                    text2 = "There was a mean ogre in the forest,\n",
                    text3 = "So, avoid the forest.\n",
                    filename = "/tmp/funnybunny.txt";

std::vector< char > buf( 1 + std::max(
                                text1.size(),
                                std::max( text2.size(), text3.size() )
                            ) );

long strcmp_chomp( const std::string& str )
{
    return strcmp( & buf.front(), str.substr( 0, str.size() - 1 ).c_str() );
}

int test_x( ivi::File_Ptr& f1, const std::string& fname )
{
    // write some crap
    long    c1 = ivi_c::ivi_fputs( f1, text1.c_str() ),
            c2 = ivi_c::ivi_fputs( f1, text2.c_str() ),
            c3 = ivi_c::ivi_fputs( f1, text3.c_str() );

    assert( '\n' == text1[ text1.size() - 1 ] );
    assert( '\n' == text2[ text2.size() - 1 ] );
    assert( '\n' == text3[ text3.size() - 1 ] );
    assert( c1 == long( text1.size() ) );
    assert( c2 == long( text2.size() ) );
    assert( c3 == long( text3.size() ) );

    fflush( f1 );

    // read it back in by reopening the already-open file, which should be cool
    ivi::File_Ptr_Read f2( fname );

    // fget_line does NOT append the newline char, unlike our old friend fgets.
    // That is why the assertions below have equality off by one.
    long    d1 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e1 = strcmp_chomp( text1 ),
            d2 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e2 = strcmp_chomp( text2 ),
            d3 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e3 = strcmp_chomp( text3 );

    assert( 1 + d1 == c1 );
    assert( 1 + d2 == c2 );
    assert( 1 + d3 == c3 );
    assert( 0 == e1 );
    assert( 0 == e2 );
    assert( 0 == e3 );

    return EXIT_SUCCESS;
}

int test1()
{
    ivi::File_Ptr_Write f1( filename );
    return test_x( f1, filename );
}

int test2()
{
    ivi::Temporary_File tf;
    return test_x( tf, tf.get_filename() );
}

// similar to test_x but with the same file pointer, since it was mode "r+"
int test3()
{
    ivi::Temporary_File tf;
    long    c1 = ivi_c::ivi_fputs( tf, text1.c_str() ),
            c2 = ivi_c::ivi_fputs( tf, text2.c_str() ),
            c3 = ivi_c::ivi_fputs( tf, text3.c_str() );

    assert( c1 == long( text1.size() ) );
    assert( c2 == long( text2.size() ) );
    assert( c3 == long( text3.size() ) );
    rewind( tf );

    long    d1 = ivi_c::fget_line( tf, & buf.front(), buf.size() ),
            e1 = strcmp_chomp( text1 ),
            d2 = ivi_c::fget_line( tf, & buf.front(), buf.size() ),
            e2 = strcmp_chomp( text2 ),
            d3 = ivi_c::fget_line( tf, & buf.front(), buf.size() ),
            e3 = strcmp_chomp( text3 );

    assert( 1 + d1 == c1 );
    assert( 1 + d2 == c2 );
    assert( 1 + d3 == c3 );
    assert( 0 == e1 );
    assert( 0 == e2 );
    assert( 0 == e3 );

    return EXIT_SUCCESS;
}

void cat( ivi::File_Ptr& fp )
{
    for( int c ; ( c = std::getc( fp ) ) != EOF; )
        std::putchar( c );
}

int test4()
{
    ivi::File_Ptr_Write f( filename );
    ivi_c::ivi_fputs( f, text1.c_str() );
    ivi_c::ivi_fputs( f, text2.c_str() );
    ivi_c::ivi_fputs( f, text3.c_str() );
    f.close();

    ivi_c::ivi_system( ( "bzip2 " + filename ).c_str() );

    bool caught = false;
    try {
        ivi::File_Ptr_Read f( filename );
        // next line is not reached
        cat( f );
    }
    catch ( ivi::IO_error& ) {
        caught = true;
    }
    assert( caught );

    ivi::File_Ptr_Smart_Read f2( filename );

    long    d1 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e1 = strcmp_chomp( text1 ),
            d2 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e2 = strcmp_chomp( text2 ),
            d3 = ivi_c::fget_line( f2, & buf.front(), buf.size() ),
            e3 = strcmp_chomp( text3 );

    assert( 1 + d1 == long( text1.size() ) );
    assert( 1 + d2 == long( text2.size() ) );
    assert( 1 + d3 == long( text3.size() ) );
    assert( 0 == e1 );
    assert( 0 == e2 );
    assert( 0 == e3 );

    return EXIT_SUCCESS;
}


}

int main( int, char** )
{
    ivi_c::ivi_init();

    IVI( EPETE( test1() != EXIT_SUCCESS ) );
    IVI( EPETE( ivi_c::ivi_unlink( filename.c_str() ) ) );

    IVI( EPETE( test2() != EXIT_SUCCESS ) );
    IVI( EPETE( test3() != EXIT_SUCCESS ) );
    IVI( EPETE( test4() != EXIT_SUCCESS ) );
    IVI( EPETE( ivi_c::ivi_unlink( ( filename + ".bz2" ).c_str() ) ) );

    ivi_c::ivi_cleanup();
    return EXIT_SUCCESS;
}

