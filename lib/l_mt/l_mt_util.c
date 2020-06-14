/*
 * $Id: l_mt_util.c 25499 2020-06-14 13:26:04Z kobus $
 */

#include "l/l_sys_debug.h"
#include <l/l_sys_mal.h>
#include <l_mt/l_mt_pthread.h>
#include <l_mt/l_mt_util.h>


/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_malloc
 *
 * Threadsafe memory allocation
 *
 * This allocates memory using libivi routines in a threadsafe way, because
 * it uses the multithread wrapper serialization lock.  Only one thread at a
 * time can malloc memory via this function.  In all other respects this
 * operates like ivi_malloc.
 *
 * Returns:
 *      NULL if allocation is unsuccessful, along with an error message.
 *      Otherwise, this returns a pointer to the memory block of the requested
 *      size.
 *
 * Related: ivi_malloc, ivi_free, ivi_mt_free
 *
 * Index: threads, memory allocation
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
void *ivi_mt_malloc(Malloc_size num_bytes)
{
    void *p;
    ivi_multithread_wrapper_serialization_lock();
    p = ivi_malloc(num_bytes);
    ivi_multithread_wrapper_serialization_unlock();
    return p;
}




/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_free
 *
 * Free heap memory, thread-safely
 *
 * This frees a block of heap memory allocated using ivi_mt_malloc,
 * ivi_malloc, or any other libivi memory allocation function.  This works
 * like ivi_free except that the operation is threadsafe, because it
 * is serialized with the multithread wrapper serialization lock.
 * Only one thread at a time therefore can free memory via this function.
 *
 * Related: ivi_malloc, ivi_free, ivi_mt_malloc
 *
 * Index: threads, memory allocation
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
void ivi_mt_free(void *ptr)
{
    ivi_multithread_wrapper_serialization_lock();
    ivi_free(ptr);
    ivi_multithread_wrapper_serialization_unlock();
}




/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_fopen
 *
 * Open a file stream, thread-safely
 *
 * This opens a file stream using libivi routines in a threadsafe way, because
 * it uses the multithread wrapper serialization lock.  Only one thread at a
 * time can open a file via this function.  In all other respects this
 * operates like ivi_fopen.
 *
 * Returns:
 *     Pointer to FILE if successful, otherwise NULL and an error message is
 *     set.
 *
 * Related:  ivi_fopen, ivi_mt_fclose, ivi_mt_fwrite, ivi_mt_fprintf
 *
 * Index: threads, I/O
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
FILE *ivi_mt_fopen(
    const char *input_fd_name,
    const char *mode
)
{
    FILE* fp;
    ivi_multithread_wrapper_serialization_lock();
    fp = ivi_fopen(input_fd_name, mode);
    ivi_multithread_wrapper_serialization_unlock();
    return fp;
}



/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_fread
 *
 * Read from a file stream, thread-safely
 *
 * This reads from file stream using libivi routines in a threadsafe way,
 * because
 * it uses the multithread wrapper serialization lock.  Only one thread at a
 * time can read from a file via this function.  In all other respects this
 * operates like ivi_fread.
 *
 * Returns:
 *     Number of bytes read, or ERROR and an error message is set.
 *
 * Related:  ivi_mt_fopen, ivi_mt_fclose, ivi_mt_fwrite, ivi_mt_fprintf,
 *           ivi_fread
 *
 * Index: threads, I/O, input
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
long ivi_mt_fread(
    FILE *fp,
    void *buff,
    size_t len
)
{
    long ct;
    ivi_multithread_wrapper_serialization_lock();
    ct = ivi_fread(fp, buff, len);
    ivi_multithread_wrapper_serialization_unlock();
    return ct;
}



/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_fwrite
 *
 * Write to a file stream, thread-safely
 *
 * This writes to file stream using libivi routines in a threadsafe way,
 * because
 * it uses the multithread wrapper serialization lock.  Only one thread at a
 * time can write to a file via this function.  In all other respects this
 * operates like ivi_fwrite.
 *
 * Returns:
 *     Number of bytes written, or ERROR and an error message is set.
 *
 * Related:  ivi_mt_fopen, ivi_mt_fclose, ivi_fwrite, ivi_mt_fprintf,
 *           ivi_mt_fread
 *
 * Index: threads, I/O, output
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
long ivi_mt_fwrite(
    FILE *fp,
    const void *line,
    size_t len
)
{
    long ct;
    ivi_multithread_wrapper_serialization_lock();
    ct = ivi_fwrite(fp, line, len);
    ivi_multithread_wrapper_serialization_unlock();
    return ct;
}




/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_fprintf
 *
 * Print to a file stream, thread-safely
 *
 * This prints formatted data to file stream using libivi routines in a
 * threadsafe way, because it uses the multithread wrapper serialization lock.
 * Only one thread at a time can write to a file via this function.
 * In all other respects this operates like ivi_fprintf.
 *
 * Returns:
 *     Number of chars written, or ERROR and an error message is set.
 *
 * Related:  ivi_mt_fopen, ivi_mt_fclose, ivi_mt_fwrite, ivi_fprintf,
 *           ivi_mt_fread
 *
 * Index: threads, I/O, output
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
long ivi_mt_fprintf(FILE* fp, const char* format_str, ...)
{
    int result;
    va_list ap;

    ivi_multithread_wrapper_serialization_lock();
    va_start(ap, format_str);
    result = ivi_vfprintf(fp, format_str, ap);
    va_end(ap);
    ivi_multithread_wrapper_serialization_unlock();

    return result;
}




/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\  */

/* ============================================================================
 *                             ivi_mt_fclose
 *
 * Closes a file stream
 *
 * This closes a file stream using libivi routines in a threadsafe way, because
 * it uses the multithread wrapper serialization lock.  Only one thread at a
 * time can close a file via this function.  In all other respects this
 * operates like ivi_fclose.
 *
 * Related:  ivi_mt_fopen, ivi_fclose, ivi_mt_fwrite, ivi_mt_fprintf,
 *           ivi_mt_fread
 *
 * Returns:
 *     NO_ERROR if successful, otherwise ERROR and an error message is set.
 *
 * Index: threads, I/O
 *
 * Author:  Andrew Predoehl
 *
 * Documenter:  Andrew Predoehl
 *
 * ----------------------------------------------------------------------------
*/
int ivi_mt_fclose(FILE *fp)
{
    int result;
    ivi_multithread_wrapper_serialization_lock();
    result = ivi_fclose(fp);
    ivi_multithread_wrapper_serialization_unlock();
    return result;
}

