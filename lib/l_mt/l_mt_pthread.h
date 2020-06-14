/*
 * $Id: l_mt_pthread.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef L_MT_PTHREAD_H_LIBIVI_INCLUDED
#define L_MT_PTHREAD_H_LIBIVI_INCLUDED 1

#include <l/l_sys_def.h>

#ifdef IVI_HAVE_PTHREAD
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#ifdef IVI_HAVE_PTHREAD

/* -------------------------------------------------------------------------- */

/* ============================================================================
 *                             ivi_pthread_t
 *
 * Opaque thread identity structure
 *
 * This wraps up the POSIX thread (pthread) entity pthread_t for use in
 * the library.  The definition here is subject to change:  we could add
 * more fields in the future, so don't assume this will always be just
 * a simple typedef.
 *
 * Index: threads
 *
 * ----------------------------------------------------------------------------
*/
typedef pthread_t               ivi_pthread_t;

typedef pthread_attr_t          ivi_pthread_attr_t;

typedef pthread_mutex_t         ivi_pthread_mutex_t;

typedef pthread_mutexattr_t     ivi_pthread_mutexattr_t;

typedef pthread_key_t           ivi_pthread_key_t;

typedef pthread_once_t          ivi_pthread_once_t;

/* Mutex initializers */
#define IVI_PTHREAD_MUTEX_INITIALIZER \
                PTHREAD_MUTEX_INITIALIZER

/* The cleanup push and cleanup pop operations are often macros. */
#define ivi_pthread_cleanup_push(f, x) \
                pthread_cleanup_push(f, x)
#define ivi_pthread_cleanup_pop(x) \
                pthread_cleanup_pop(x)

#define IVI_PTHREAD_KEYS_MAX \
                ((PTHREAD_KEYS_MAX) - 1)

#define IVI_PTHREAD_ONCE_INIT \
                PTHREAD_ONCE_INIT

#define IVI_PTHREAD_CREATE_JOINABLE \
                PTHREAD_CREATE_JOINABLE
#define IVI_PTHREAD_CREATE_DETACHED \
                PTHREAD_CREATE_DETACHED


#else    /* ############################################################## */
/*
 * FAKE STUFF!  Use this when the system lacks pthread.h or for NO_LIBS builds.
 */

typedef void*                                         ivi_pthread_t;
typedef void*                                         ivi_pthread_attr_t;
typedef void*                                         ivi_pthread_mutex_t;
typedef void*                                         ivi_pthread_mutexattr_t;
typedef int                                           ivi_pthread_key_t;
typedef int                                           ivi_pthread_once_t;
#define IVI_PTHREAD_MUTEX_INITIALIZER                 NULL
#define ivi_pthread_cleanup_push(f, x)                do { ; } while(0)
#define ivi_pthread_cleanup_pop(x)                    do { ; } while(0)
#define IVI_PTHREAD_KEYS_MAX                          0
#define IVI_PTHREAD_ONCE_INIT                         0
#define IVI_PTHREAD_CREATE_JOINABLE                   0
#define IVI_PTHREAD_CREATE_DETACHED                   0

#endif /* IVI_HAVE_PTHREAD */



/* -------------------------------------------------------------------------
 * Stuff below this line should compile regardless of whether the system
 * has included pthread.h.
 * -------------------------------------------------------------------------
 */


/* BASIC PTHREAD CONTROL function ............................... */

int ivi_pthread_create(
    ivi_pthread_t* newthread,
    const ivi_pthread_attr_t* attr,
    void* (*start_routine)(void*),
    void* arg
);

int ivi_pthread_join(ivi_pthread_t tid, void** rval_p);

void ivi_pthread_exit(void* rval_p);



/* ADVANCED PTHREAD functions ................................... */

int get_ivi_pthread_self(ivi_pthread_t* tid);

int ivi_pthread_equal(ivi_pthread_t tid1, ivi_pthread_t tid2);

int ivi_pthread_cancel(ivi_pthread_t tid);

int ivi_pthread_attr_init(ivi_pthread_attr_t* attr_p);

int ivi_pthread_attr_destroy(ivi_pthread_attr_t* attr_p);

int ivi_pthread_attr_setdetachstate(ivi_pthread_attr_t* attr_p, int state);

int ivi_pthread_attr_getdetachstate(ivi_pthread_attr_t* attr_p, int* state);



/* MUTEX functions .............................................. */

int ivi_pthread_mutex_lock(ivi_pthread_mutex_t* mutex);

int ivi_pthread_mutex_trylock(ivi_pthread_mutex_t* mutex);

int ivi_pthread_mutex_unlock(ivi_pthread_mutex_t* mutex);

int ivi_pthread_mutex_init(
    ivi_pthread_mutex_t* mutex,
    ivi_pthread_mutexattr_t* attr
);

int ivi_pthread_mutex_destroy(ivi_pthread_mutex_t* mutex);



/* THREAD SPECIFIC DATA functions ................................ */

int ivi_pthread_key_create(
    ivi_pthread_key_t *key,
    void (*destr_function) (void *)
);

int ivi_pthread_key_delete(ivi_pthread_key_t key);

int ivi_pthread_setspecific(ivi_pthread_key_t key, const void *pointer);

void* ivi_pthread_getspecific(ivi_pthread_key_t key);

int ivi_pthread_once(
    ivi_pthread_once_t* once_control,
    void (*init_routine)(void)
);



/* Meta-wrapper functions:  they concern the wrapper itself.  Use sparingly.
   ................................................................ */

int ivi_multithread_wrapper_serialization_lock(void);

int ivi_multithread_wrapper_serialization_unlock(void);

int ivi_pthread_read_prng_seeds(ivi_uint16* s, size_t buf_length);

int get_ivi_pthread_number(void);

void reset_ivi_pthread_counter(void);

/* below:  end-of-file boilerplate */

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif
