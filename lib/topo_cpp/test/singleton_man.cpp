/**
 * @brief test for proper singleton behavior from class Tile_manager
 * @file
 * @author Andrew Predoehl
 *
 * Class Tile_manager is supposed to use the "singleton" pattern, i.e., only
 * one object can be instantiated from this class.  But does it really enforce
 * that stipulation?  Let's find out!
 *
 * Hurray for test programs:  I discovered a file pointer leak in topo/master.c
 * with this program.
 */
/*
 * $Id: singleton_man.cpp 25499 2020-06-14 13:26:04Z kobus $
 */
#include <l/l_init.h>
#include <l/l_sys_io.h>
#include <l/l_sys_lib.h>
#include <l_cpp/l_test.h>
#include <topo/index.h>
#include <topo_cpp/dorthoquad.h>

int test1()
{
    // instantiate it once:  shame on you.
    ivi::TopoFusion::Tile_manager t1;

    // instantiate it twice:  shame on me.
    TEST_FAIL( ivi::TopoFusion::Tile_manager t2 );
    TEST_FAIL( ivi::TopoFusion::Tile_manager t2("foo") );

    return ivi_c::NO_ERROR;
}

int main()
{
    IVI(EPETE(ivi_init()));
    IVI(EPETE(test1()));
    ivi_c::ivi_cleanup();
    RETURN_VICTORIOUSLY();
}

