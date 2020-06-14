/*
 * $Id: test_basic_threads.c 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_lib.h>
#include <l/l_sys_io.h>
#include <l/l_error.h>
#include <l/l_global.h>
#include <l/l_init.h>
#include <l_mt/l_mt_pthread.h>

#define HIVE_SIZE 50

ivi_pthread_mutex_t mtx = IVI_PTHREAD_MUTEX_INITIALIZER;

static void* thready_work(void* p)
{
    union { void* v; int i; } u;
    u.v = p;

    EPETE(ivi_pthread_mutex_lock(&mtx));
    ivi_printf("worker thread %d\n", u.i);
    EPETE(ivi_pthread_mutex_unlock(&mtx));

    return NULL;
}

int main(int argc, char** argv)
{
    int i;
    ivi_pthread_t worker_bees[HIVE_SIZE];

    if (! is_interactive()) 
    {
        p_stderr("This test program either needs to be adjusted for batch testing or moved.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    

    EPETE(ivi_init());
    ivi_puts("home thread hello\n");

    /* start them */
    for (i = 0; i < HIVE_SIZE; ++i)
    {
        EPETE(ivi_pthread_create(worker_bees+i, NULL, thready_work, (void*)i));
    }

    EPETE(ivi_pthread_mutex_lock(&mtx));
    ivi_puts("all threads launched\n");
    EPETE(ivi_pthread_mutex_unlock(&mtx));

    /* stop them */
    for (i = 0; i < HIVE_SIZE; ++i)
    {
        EPETE(ivi_pthread_join(worker_bees[i], NULL));
    }
    ivi_cleanup();

    return EXIT_SUCCESS;
}

