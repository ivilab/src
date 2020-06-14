/**
 * @file
 * @author Andy Predoehl
 * @brief unit test for Temporary_Directory class
 */
/*
 * $Id: test_tempdir.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_lib.h>
#include <l/l_debug.h>
#include <l/l_init.h>
#include <l_cpp/l_stdio_wrap.h>
#include <l_cpp/l_util.h>

namespace
{

int main2()
{
    // test that it creates and destroys an empty directory (and, get_pathname)
    std::string name, filenam = "funnybunny";
    if (1)
    {
        ivi::Temporary_Directory td;
        name = td.get_pathname();
        IVI( ASSERT( is_directory( name.c_str() ) ) );
    }
    IVI( ASSERT( ! is_directory( name.c_str() ) ) );

    // test the remove method
    if (1)
    {
        ivi::Temporary_Directory td;
        name = td.get_pathname();
        IVI( ASSERT( is_directory( name.c_str() ) ) );
        IVI( EPETE( td.remove() ) );
        IVI( ASSERT( ! is_directory( name.c_str() ) ) );
    }
    IVI( ASSERT( ! is_directory( name.c_str() ) ) );

    // test that it cannot destroy a nonempty directory
    if (1)
    {
        ivi::Temporary_Directory td;
        name = td.get_pathname();
        IVI( ASSERT( is_directory( name.c_str() ) ) );
        ivi::File_Ptr_Write fp( name + DIR_STR + filenam );
        ivi_c::ivi_fputs( fp, "hello dolly" );
        if (ivi_c::is_interactive())
        {
            TEST_PSE(("PLEASE DISREGARD THE FOLLOWING ERROR MESSAGE!\n"));
        }
        else // automated test gets confused by the always-changing random
        {    // letters in the tempdir name, so throw away the message.
            td.set_silence_flag();
        }
    }
    IVI( ASSERT( is_directory( name.c_str() ) ) );
    // ok we've proved our point, so clean it up manually
    IVI( EPETE( ivi_unlink( (name + DIR_STR + filenam).c_str() ) ) );
    IVI( EPETE( ivi_rmdir( name.c_str() ) ) );

    // test that the nuke option destroys even a nonempty directory
    if (1)
    {
        ivi::Temporary_Directory td;
        name = td.get_pathname();
        IVI( ASSERT( is_directory( name.c_str() ) ) );
        ivi::File_Ptr_Write fp( name + DIR_STR + filenam );
        ivi_c::ivi_fputs( fp, "hello dolly" );
        IVI( ASSERT( is_directory( name.c_str() ) ) );
        IVI( EPETE( td.recursively_remove() ) );
        IVI( ASSERT( ! is_directory( name.c_str() ) ) );
    }
    IVI( ASSERT( ! is_directory( name.c_str() ) ) );

    // test self-nuking
    if (1)
    {
        ivi::Temporary_Recursively_Removing_Directory td;
        name = td.get_pathname();
        IVI( ASSERT( is_directory( name.c_str() ) ) );
        ivi::File_Ptr_Write fp( name + DIR_STR + filenam );
        ivi_c::ivi_fputs( fp, "hello dolly" );
    }
    IVI( ASSERT( ! is_directory( name.c_str() ) ) );

    return ivi_c::NO_ERROR;
}

}

int main ( int argc, char** argv )
{
    ivi_c::ivi_init();

    try {
        IVI( EPETE( main2() ) );
    }
    catch( ivi::Exception& e )
    {
        e.print_details_exit();
    }

    ivi_c::ivi_cleanup();
    return EXIT_SUCCESS;
}

