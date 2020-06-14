/**
 * @file
 * @brief simple raii class to manage lock/unlock of a pthread mutex
 * @author Andrew Predoehl
 */
/*
 * $Id: l_mt_mutexlock.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef MUTEXLOCK_H_INCLUDED_IVILAB_2013AUG09
#define MUTEXLOCK_H_INCLUDED_IVILAB_2013AUG09

#include <l_mt/l_mt_pthread.h>
#include <l_cpp/l_exception.h>

namespace ivi
{

/**
 * @defgroup iviThreads Thread support
 *
 * This group contains classes that support multi-threaded programs,
 * at a low level (like mutexes) and at a high level (like partially
 * thread-safe, paralleized classes for fast convolution).
 *
 * @{
 */


class Mutex_lock;

/**
 * @brief dynamically allocated mutex:  unlock before you destroy it!
 *
 * This object is not copyable or assignable, but it does let you dynamically
 * create a mutex.
 *
 * You cannot store these objects in a container, because they are 
 * uncopyable.  However, you can store shared pointers to them easily, e.g.:
 *
 * @code
 * #include <boost/shared_ptr.hpp>
 * #include <boost/make_shared.hpp>
 * std::vector<boost::shared_ptr<ivi::Pthread_mutex> > foo_lock;
 * while (foo_lock.size() < PROPER_SIZE)
 * {
 *     foo_lock.push_back(boost::make_shared<ivi::Pthread_mutex>());
 * }
 * @endcode
 *
 * @warning You are obliged to make sure the mutex is unlocked before
 * you destroy it!
 *
 * @see Pthread_locked_mutex
 */
class Pthread_mutex
{
    ivi_c::ivi_pthread_mutex_t mutex_;

    Pthread_mutex(const Pthread_mutex&);            // uncopyable
    Pthread_mutex& operator=(const Pthread_mutex&); // unassignable

    friend class Mutex_lock;

    void fp_abort(const char*);

public:
    /// @brief dynamic mutex:  unlock before you destroy it!
    Pthread_mutex(void (Pthread_mutex::* pmf)() = 0)
    {
#ifndef IVI_HAVE_PTHREAD
        IVI_THROW_2(Missing_dependency, "built without pthread");
#endif
        ETX(ivi_c::ivi_pthread_mutex_init(&mutex_, 0));

        if (pmf)
        {
            (this ->* pmf)();
        }
    }

    /// @brief dtor cleans up the mutex, which MUST be unlocked beforehand.
    virtual ~Pthread_mutex()
    {
        if (ivi_c::ERROR == ivi_c::ivi_pthread_mutex_destroy(&mutex_))
        {
            fp_abort(ivi::ivi_get_error().c_str());
        }
    }

    /// @brief lock this mutex
    void lock()
    {
        ETX(ivi_c::ivi_pthread_mutex_lock(&mutex_));
    }

    /// @brief unlock this mutex
    void unlock()
    {
        ETX(ivi_c::ivi_pthread_mutex_unlock(&mutex_));
    }

    /// @brief try to lock; return WOULD_BLOCK if locked, else NO_ERROR.
    int try_lock()
    {
        return ivi_c::ivi_pthread_mutex_trylock(&mutex_);
    }
};


/// @brief same as Pthread_mutex, but starts off in "locked" state.
struct Pthread_locked_mutex : public Pthread_mutex
{
    Pthread_locked_mutex() : Pthread_mutex(& Pthread_mutex::lock) {}
};


/**
 * @brief simple RAII class to grab and release a mutex
 *
 * All this does is lock a mutex at creation time, and guarantee that it gets
 * unlocked, unless an abort occurs.  The ctor accepts either a pointer to a
 * ivi_c::ivi_pthread_t object (the C-language style struct used by the
 * multithread wrapper), or a C++ object, either a Pthread_mutex reference
 * or pointer (it does not matter which).
 *
 * If you wish to unlock the mutex prior to the destruction of this object,
 * just call the release() method.  It is safe to call release() multiple times
 * on the same object -- only the first one has effect, the rest are
 * benign NOPs.  However, if for some reason the mutex cannot be unlocked, the
 * object's destructor will abort the program.
 */
class Mutex_lock
{
    ivi_c::ivi_pthread_mutex_t* mutex_;
    Mutex_lock(const Mutex_lock&);              // uncopyable
    Mutex_lock& operator=(const Mutex_lock&);   // unassignable

    void lock()
    {
        NTX(mutex_);
        ETX(ivi_c::ivi_pthread_mutex_lock(mutex_));
    }

    void fp_abort(const char*);

public:
    /// @brief establish a lock on the mutex (as a C-style struct)
    Mutex_lock(ivi_c::ivi_pthread_mutex_t* m)
    :   mutex_(m)
    {
        lock();
    }

    /// @brief establish a lock on the mutex object (as a reference)
    Mutex_lock(Pthread_mutex& m)
    :    mutex_(& m.mutex_)
    {
        lock();
    }

    /// @brief establish a lock on the mutex object pointer
    Mutex_lock(Pthread_mutex* m)
    :    mutex_(m ? & m -> mutex_ : 0)
    {
        lock();
    }

    /// @brief release the mutex for others (safe to call twice)
    void release()
    {
        if (mutex_)
        {
            if (ivi_c::ERROR == ivi_c::ivi_pthread_mutex_unlock(mutex_))
            {
                fp_abort("unlock");
            }
            mutex_ = 0;
        }
    }

    /// @brief dtor releases the mutex
    ~Mutex_lock()
    {
        release();
    }
};


/// @}

}


#endif
