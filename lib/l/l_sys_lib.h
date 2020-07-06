
/* $Id: l_sys_lib.h 25612 2020-07-06 22:17:23Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author).
|
|  Personal and educational use of this code is granted, provided that this
|  header is kept intact, and that the authorship is not misrepresented, that
|  its use is acknowledged in publications, and relevant papers are cited.
|
|  For other use contact the author (kobus AT cs DOT arizona DOT edu).
|
|  Please note that the code in this file has not necessarily been adequately
|  tested. Naturally, there is no guarantee of performance, support, or fitness
|  for any particular task. Nonetheless, I am interested in hearing about
|  problems that you encounter.
|
* =========================================================================== */

#ifndef L_SYS_LIB_INCLUDED
#define L_SYS_LIB_INCLUDED

/* Start for c2man */


#include "l/l_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif


/* -------------------------------------------------------------------------- */

#ifdef TEST
#    define  add_cleanup_function(x)  \
                        debug_add_cleanup_function(x, __FILE__, __LINE__)
#endif

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/* =============================================================================
 *                             BUFF_GET_ENV
 *
 * (MACRO) Sets up call to ivi_get_env
 *
 * The max_len parameter is set to sizeof(env_var_val).  Using sizeof to set the
 * buffer size is recomended where applicable, as the code will not be broken
 * if the buffer size changes. HOWEVER, neither this method, nor the macro, is
 * applicable if line is NOT a character array.  If line is declared by "char
 * *line", then the size of line is the number of bytes in a character pointer
 * (usually 4), which is NOT what is normally intended.  You have been WARNED!
 *
 * Related:
 *    ivi_get_env
 *
 * Index: Macros, standard library
 *
 * -----------------------------------------------------------------------------
*/
#ifdef __C2MAN__    /* Pretend we are a ".c" file to document MACROS. */
    int BUFF_GET_ENV(const char* env_var, char env_var_val[ ]);
#else
#   define BUFF_GET_ENV(x,y)              ivi_get_env(x, y, sizeof(y ))
#endif

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/* =============================================================================
 *                             BUFF_GET_USER_ID
 *
 * (MACRO) Sets up call to get_user_id
 *
 * The max_len parameter is set to sizeof(user_id).  Using sizeof to set the
 * buffer size is recomended where applicable, as the code will not be broken
 * if the buffer size changes. HOWEVER, neither this method, nor the macro, is
 * applicable if line is NOT a character array.  If line is declared by "char
 * *line", then the size of line is the number of bytes in a character pointer
 * (usually 4), which is NOT what is normally intended.  You have been WARNED!
 *
 * Related:
 *    get_user_id
 *
 * Index: Macros, standard library
 *
 * -----------------------------------------------------------------------------
*/
#ifdef __C2MAN__    /* Pretend we are a ".c" file to document MACROS. */
    int BUFF_GET_USER_ID(char user_id[ ]);
#else
#    define BUFF_GET_USER_ID(x)            get_user_id(x, sizeof(x ))
#endif

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/* =============================================================================
 *                             BUFF_GET_TEMP_FILE_NAME
 *
 * (MACRO) Sets up call to get_temp_file_name
 *
 * The max_len parameter is set to sizeof(temp_name).  Using sizeof to set the
 * buffer size is recomended where applicable, as the code will not be broken
 * if the buffer size changes. HOWEVER, neither this method, nor the macro, is
 * applicable if line is NOT a character array.  If line is declared by "char
 * *line", then the size of line is the number of bytes in a character pointer
 * (usually 4), which is NOT what is normally intended.  You have been WARNED!
 *
 * Related:
 *    get_temp_file_name
 *
 * Index: Macros, standard library
 *
 * -----------------------------------------------------------------------------
*/
#ifdef __C2MAN__    /* Pretend we are a ".c" file to document MACROS. */
    int BUFF_GET_TEMP_FILE_NAME(char temp_name[ ]);
#else
#    define BUFF_GET_TEMP_FILE_NAME(x)     get_temp_file_name(x, sizeof(x))
#endif

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

int  set_lock_options
(
    const char* option,
    const char* value
);

int  check_for_freeze              (void);
int  is_interactive                (void);
int  get_user_id                   (char*, size_t);
int  get_group                     (char*, size_t);
int  ivi_get_env                   (const char*, char*, size_t);
int  create_system_command_process (void);
void destroy_system_command_process(void);
int  ivi_system                    (const char* command);
int  ivi_system_2                  (const char* command, int* prog_rc_ptr);

#ifdef UNIX
    int ivi_exec(const char*);
    int ivi_waitpid(pid_t);

    int ivi_waitpid2
    (
        pid_t pid,
        int*  exit_status_ptr,
        int*  termination_signal_ptr
    );

    int check_child(pid_t pid);

    int check_child2
    (
        pid_t pid,
        int*  exit_status_ptr,
        int*  termination_signal_ptr
    );
#endif

int process_is_alive(int pid);
int get_idle_time(void);


void terminate_child_process(pid_t pid);

void arrange_cleanup(void);

#ifdef TEST
    int debug_add_cleanup_function
    (
        void        (*cleanup_fn)(void),
        const char* file_name,
        int         line_number
    );
#else
    int add_cleanup_function(void (*cleanup_fn)(void));
#endif

void reset_cleanup_for_fork(void); 

void  ivi_cleanup          (void);
void  ivi_cleanup_for_abort(void);
void  ivi_optional_abort   (void);
void  ivi_abort            (void);
void  ivi_exit             (int);
void  ivi_exit_2           (int);
pid_t ivi_fork             (void);
int   get_temp_file_name   (char* temp_file_name, size_t max_len);

#ifdef TEST
    int test_process_is_alive(void);
#endif


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

/* End for c2man */

#endif

