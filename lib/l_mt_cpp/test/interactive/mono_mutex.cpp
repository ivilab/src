/*
 * This demonstrates an "automatic" mutex, i.e., it lives on the stack, not in
 * static memory, and it is locked and unlocked by ten threads.
 *
 * $Id: mono_mutex.cpp 25499 2020-06-14 13:26:04Z kobus $
 */
#include <l/l_incl.h>
#include <l_cpp/l_util.h>
#include <l_mt_cpp/l_mt_mutexlock.h>

namespace
{

// non-reentrant function; we serialize calls to it using the mutex.
void put()
{
    static int i;
    ivi_c::ivi_printf("print line %4d   (called by thread %d)\n",
                      i++, ivi_c::get_ivi_pthread_number());

    // nap does not work -- I guess signals and threads do not mix.
    //ivi_c::nap(10 * ivi_c::ivi_rand());
}

// this enforces serial access to the above thread-unsafe function put().
void serial_put(ivi::Pthread_mutex* q)
{
    ivi::Mutex_lock m(q);   /* lock the mutex                           */
    put();                  /* call the "critical" function             */
                            /* mutex is automatically unlocked by dtor  */
}

// thread worker function, which is given a pointer to mutex as a parameter
void* work(void* v)
{
    ivi::Pthread_mutex* q = static_cast<ivi::Pthread_mutex*>(v);
    IVI(NRN(q));

    serial_put(q);
    serial_put(q);
    serial_put(q);

    return q;
}

// test function launches ten threads and joins them up again.
int test()
{
    const int TCT = 10;
    ivi_c::ivi_pthread_t tid[TCT];
    ivi::Pthread_mutex q;

    for (int i = 0; i < TCT; ++i)
    {
        IVI(ERE(ivi_pthread_create(tid + i, NULL, & work, & q)));
    }
    for (int i = 0; i < TCT; ++i)
    {
        void *v;
        IVI(ERE(ivi_pthread_join(tid[i], &v)));
        IVI(NRE(v));
    }

    return ivi_c::NO_ERROR;
}

}

// main function starts the library nanny, catches errors, cleans up.
int main(int argc, char** argv)
{
    IVI(EPETE(ivi_init()));
    try
    {
        IVI(EPETE(test()));
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }
    ivi_c::ivi_cleanup();
    return EXIT_SUCCESS;
}

