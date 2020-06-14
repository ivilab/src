/**
 * @file
 * @author Andrew Predoehl
 * @brief def of RAII class to control a pthread attr object
 */
/*
 * $Id: l_mt_pt_attr.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef PT_ATTR_H_INCLUDED_IVILAB
#define PT_ATTR_H_INCLUDED_IVILAB

#include <l_mt/l_mt_pthread.h>
#include <l_cpp/l_exception.h>

namespace ivi
{

/**
 * @addtogroup iviThreads Thread support
 * @{
 */

/**
 * @brief RAII class to manage an object of type ivi_pthread_attr_t
 *
 * The ivi_pthread sublibrary, which wraps the Posix pthread library,
 * uses an object of type ivi_pthread_attr_t to describe the attributes
 * of a thread.  Thread attributes are its "detach state," its stack
 * size, and other things.  See pthread_attr_init(3).
 * A single attribute object may be used on multiple threads.
 *
 * The attribute object must be created and destroyed, thus this RAII
 * class exists to manage those tasks.
 * Objects of this class are not copyable or assignable.
 *
 * Currently this class does not support all thread attribute options,
 * but a developer is welcome to add new ones to the C library wrapper
 * (l_mt) and add WRAPPED calls through this class.  Please do not call
 * pthreads functions directly -- use the wrapped calls in ivi_c.
 *
 * Example.
 * @code
 * void* ha(void*) { for(int i=100; i--; ) puts("lol"); }
 * main() {
 *  ivi_c::ivi_pthread_t tid;
 *  ivi::Pthread_attr a;
 *  a.set_detached();
 *  ivi_c::ivi_pthread_create(*tid, a, ha, NULL);
 *  // no need to join
 *  return 0;
 * }
 * @endcode
 */
class Pthread_attr
{
    // private attribute object
    ivi_c::ivi_pthread_attr_t m_attr;

    Pthread_attr(const Pthread_attr&);  // teaser:  do not copy
    Pthread_attr& operator=(const Pthread_attr&);  // teaser:  do not assign

public:
    /// @brief initialize a generic attribute object.
    Pthread_attr()
    {
        ETX(ivi_c::ivi_pthread_attr_init(&m_attr));
    }

    /// @brief destroy an attribute object.
    ~Pthread_attr()
    {
        ivi_c::ivi_pthread_attr_destroy(&m_attr);
    }

    /**
     * @brief set the detach flag.
     * @param flag IVI_PTHREAD_CREATE_JOINABLE, IVI_PTHREAD_CREATE_DETACHED
     *             are the two valid values.
     * @return ERROR or NO_ERROR as appropriate
     * @see set_detached as a possibly more readable alternative
     */
    int setdetachstate(int state)
    {
        return ivi_c::ivi_pthread_attr_setdetachstate(&m_attr, state);
    }

    /// @brief convenience synonym setdetachstate(IVI_PTHREAD_CREATE_DETACHED)
    int set_detached()
    {
        return setdetachstate(IVI_PTHREAD_CREATE_DETACHED);
    }

    /**
     * @brief obtain the detach setting of this object.
     * @param[out] state  Pointer to int into which the state is written
     * @return ERROR or NO_ERROR
     * @post *state contains IVI_PTHREAD_CREATE_JOINABLE if the thread is
     *       joinable, or IVI_PTHREAD_CREATE_DETACHED if not.
     */
    int getdetachstate(int* state)
    {
        return ivi_c::ivi_pthread_attr_getdetachstate(&m_attr, state);
    }

    /// @brief access thread attribute as const pointer.
    operator const ivi_c::ivi_pthread_attr_t*()
    {
        return &m_attr;
    }
};

/// @}

} // end ns ivi

#endif /* PT_ATTR_H_INCLUDED_IVILAB */
