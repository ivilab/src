/**
 * @file
 * @author Andrew Predoehl
 * @brief trivial wrapper on abort
 *
 * This file would not be necessary except that the build system is not very
 * flexible about where function implementations live relative to their
 * header declarations.
 */
/*
 * $Id: l_mt_mutexlock.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_lib.h>
#include <l_mt/l_mt_util.h>
#include <l_mt_cpp/l_mt_mutexlock.h>


namespace
{

void print_string_and_abort(const char* msg)
{
    ivi_c::ivi_mt_fprintf(stderr, "%s: aborting.\n", msg);
    ivi_c::ivi_abort();
}

}


namespace ivi
{

void Mutex_lock::fp_abort(const char* s)
{
    print_string_and_abort(s);
}

void Pthread_mutex::fp_abort(const char* s)
{
    print_string_and_abort(s);
}

}
