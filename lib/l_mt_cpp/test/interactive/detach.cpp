/**
 * @file
 * @author Andrew Predoehl
 * @brief test RAII class for wrapped pthread attribute
 */
/*
 * $Id: detach.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_rand.h>
#include <l/l_sys_term.h>
#include <l/l_sys_tsig.h>
#include <l/l_error.h>
#include <l/l_global.h>
#include <l_mt/l_mt_util.h>
#include <l_cpp/l_util.h>
#include <l_mt_cpp/l_mt_pt_attr.h>

namespace
{

void* chatter(void* v)
{
    IVI(NRN(v));
    const char* m = (const char*) v;
    for(int i=100; i--; )
    {
        const int s = 1+30*ivi_c::ivi_rand();
        ivi_c::ivi_mt_fprintf(stdout, "%d %s %d\n", i, m, s);
    }
    return 0; // return value is meaningless because thread is detached
}

}

int main(int argc, char** argv)
{
    ivi_c::ivi_pthread_t tid;
    ivi::Pthread_attr a;
    a.set_detached();
    ivi_c::ivi_disable_paging();
    ivi_c::ivi_pthread_create(&tid, a, chatter, (void*) "lol");
    ivi_c::ivi_pthread_create(&tid, a, chatter, (void*) "yolo");
    ivi_c::ivi_mt_fprintf(stdout, "Parent thread is exiting soon, "
                            "you kids keep the noise down.\n");
    ivi_c::nap(500); // shutdown in .5 seconds

    // No need to join because chatter threads are detached.
    // They will be snuffed out now, whether or not they are finished.
    // Because of the lengthy nap(), very likely they are finished.
    return 0;
}
