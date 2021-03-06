
/* $Id: l_debug.c 25867 2020-10-20 15:57:20Z kobus $ */

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

#include "l/l_gen.h"     /* Only safe as first include in a ".c" file. */
#include "l/l_string.h"
#include "l/l_sys_str.h"
#include "l/l_sys_scan.h"
#include "l/l_debug.h"

#define NUM_BYTES_PER_LINE  32

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

int set_debug_options(const char* option, const char* value)
{
    char lc_option[ 100 ];
    int  temp_int_value;
    int  temp_boolean_value;
    int  result = NOT_FOUND;


    EXTENDED_LC_BUFF_CPY(lc_option, option);

    if (    (lc_option[ 0 ] == '\0')
         || match_pattern(lc_option, "debug")
         || match_pattern(lc_option, "debug-level")
       )
    {
        IMPORT int ivi_debug_level;

        if (value == NULL)
        {
            return NO_ERROR;
        }
        else if (value[ 0 ] == '?')
        {
            ERE(pso("debug-level = %d\n", ivi_debug_level));
        }
        else if (value[ 0 ] == '\0')
        {
            ERE(pso("Debug level is %d.\n", ivi_debug_level));
        }
        else
        {
            ERE(ss1i(value, &temp_int_value));
            ivi_debug_level = temp_int_value;
        }
        result = NO_ERROR;
    }

    if (    (lc_option[ 0 ] == '\0')
         || match_pattern(lc_option, "abort-interactive-math-errors")
       )
    {
        IMPORT int abort_interactive_math_errors;

        if (value == NULL)
        {
            return NO_ERROR;
        }
        else if (value[ 0 ] == '?')
        {
            ERE(pso("abort-interactive-math-errors = %s\n",
                    abort_interactive_math_errors ? "t" : "f"));
        }
        else if (value[ 0 ] == '\0')
        {
            ERE(pso("Math errors %s aborts when programs compiled with TEST are run interactively.\n",
                    abort_interactive_math_errors ? "cause" : "do not cause"));
        }
        else
        {
            ERE(temp_boolean_value = get_boolean_value(value));
            abort_interactive_math_errors = temp_boolean_value;
        }
        result = NO_ERROR;
    }

#ifdef TEST 
    if (    (lc_option[ 0 ] == '\0')
         || match_pattern(lc_option, "suppress-test-messages")
       )
    {
        IMPORT int ivi_suppress_test_messages;

        if (value == NULL)
        {
            return NO_ERROR;
        }
        else if (value[ 0 ] == '?')
        {
            ERE(pso("suppress-test-messages = %s\n",
                    ivi_suppress_test_messages ? "t" : "f"));
        }
        else if (value[ 0 ] == '\0')
        {
            ERE(pso("Error messages generated by UNTESTED_CODE, SUSPICIOUS CODE or when TEST is set %s supressed.\n",
                    ivi_suppress_test_messages ? "are" : "are not"));
        }
        else
        {
            ERE(temp_boolean_value = get_boolean_value(value));
            ivi_suppress_test_messages = temp_boolean_value;
        }
        result = NO_ERROR;
    }
#endif 

    return result;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*
 * =============================================================================
 *                              ivi_set_debug_level
 *
 * Sets the debug level for the IVI library routines.
 *
 * This routine sets the debug level for the IVI library routines. This
 * affects all calls to debug_pso(3-IVI).
 *
 * Index: I/O, debug, debugging
 *
 * -----------------------------------------------------------------------------
 */

int ivi_set_debug_level(int new_level)
{
    IMPORT int ivi_debug_level;

    ivi_debug_level = new_level;

    return NO_ERROR;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*
 * =============================================================================
 *                              ivi_get_debug_level
 *
 * Returns the debug level for the IVI library routines.
 *
 * This routine returns the debug level for the IVI library routines.
 *
 * Index: I/O, debug, debugging
 *
 * -----------------------------------------------------------------------------
 */

int ivi_get_debug_level(void)
{
    IMPORT int ivi_debug_level;

    return ivi_debug_level;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*
 * =============================================================================
 *                            set_suppress_test_messages
 *
 * Sets whether or not we suppress test messages.
 *
 * This routine sets the value of ivi_suppress_test_messages. 
 *
 * Index: I/O, debug, debugging
 *
 * -----------------------------------------------------------------------------
 */

#ifdef TEST
int set_suppress_test_messages(int new_level)
{
    IMPORT int ivi_suppress_test_messages;

    ivi_suppress_test_messages = new_level;

    return NO_ERROR;
}
#endif 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*
 * =============================================================================
 *                                  hex_print
 *
 * Prints memory in hex.
 *
 * Prints "len" bytes in the region pointed to by "input_reg_ptr" on the file
 * pointed to by "fp" in HEX. Generally only used for debugging.
 *
 * Index: debugging
 *
 * -----------------------------------------------------------------------------
 */

void hex_print(FILE* fp, void* input_reg_ptr, size_t len)
{
    size_t num_lines;
    size_t i;
    size_t j;
    size_t num_int_blocks_left;
    unsigned int*  reg_ptr;


    if (len == 0) return;

    reg_ptr = (unsigned int*)input_reg_ptr;

    num_lines = len/NUM_BYTES_PER_LINE;

    for (i=0;i<num_lines; ++i)
    {
        for (j=0; j< NUM_BYTES_PER_LINE/sizeof(int); ++j)
        {
            ivi_fprintf(fp, " %08x", *reg_ptr);
            reg_ptr++;
        }
        ivi_fprintf(fp, "\n");
    }

    num_int_blocks_left = (len % NUM_BYTES_PER_LINE) / sizeof(int);

    for (j=0; j<num_int_blocks_left; ++j)
    {
        ivi_fprintf(fp, " %08x", *reg_ptr);
        reg_ptr++;
    }
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

/*
 * =============================================================================
 *                             get_status_string
 * -----------------------------------------------------------------------------
*/

void get_status_string(int status_num, char* buff, size_t buff_size)
{

    switch (status_num)
    {
        case NO_ERROR:
            ivi_strncpy(buff, "NO_ERROR", buff_size);
            break;
        case EOF:
            ivi_strncpy(buff, "EOF", buff_size);
            break;
        case INTERRUPTED:
            ivi_strncpy(buff, "INTERRUPTED", buff_size);
            break;
        case TRUNCATED:
            ivi_strncpy(buff, "TRUNCATED", buff_size);
            break;
        case WOULD_BLOCK:
            ivi_strncpy(buff, "WOULD_BLOCK", buff_size);
            break;
        case ERROR:
            ivi_strncpy(buff, "ERROR", buff_size);
            break;
        case FOUND:
            ivi_strncpy(buff, "FOUND", buff_size);
            break;
        case NOT_FOUND:
            ivi_strncpy(buff, "NOT_FOUND", buff_size);
            break;
        case NO_SOLUTION:
            ivi_strncpy(buff, "NO_SOLUTION", buff_size);
            break;
        case NOT_SET:
            ivi_strncpy(buff, "NOT_SET", buff_size);
            break;
        case BACKGROUND_READ:
            ivi_strncpy(buff, "BACKGROUND_READ", buff_size);
            break;
        case PROCESS_IS_DEAD:
            ivi_strncpy(buff, "PROCESS_IS_DEAD", buff_size);
            break;
        default:
            ivi_sprintf(buff, buff_size, "%d", status_num);
            break;
    }
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef TEST
/*
 * =============================================================================
 *                                 test_pso
 *
 * Printing to stdout with <<TEST>> prefix
 *
 * This routine writes a formatted string to stdout with each lined prefixed
 * with <<TEST>>. Normally this routine is only used indirectly using the macro
 * TEST_PSO. Other behaviour is similar to pso. Specifically, it is similar to
 * ivi_fprintf with respect to extended formatting options and paging.
 *
 * Macros:
 *    This routine should only accessed using the macro TEST_PSO which sets
 *    things up so that the printing only occurrs if TEST is defined. If TEST
 *    is not defined, then TEST_PSO becomes nothing (IE, it is removed by the
 *    pre-processor). TEST_PSO is used as follows:
 * |    TEST_PSO(( <format>, arg1, arg2, ....)); // Note DOUBLE parens!
 *
 * Returns:
 *    Return is void, as it is normally used with the TEST_PSO macro.
 *
 * Macros:
 *    TEST_PSO
 *
 * Note:
 *    More often than not, one is better off instead using test_pso() via the
 *    macro TEST_PSO().
 *
 * Related:
 *    pse, pso, ivi_fprintf
 *
 * Index: debugging, TEST
 *
 * -----------------------------------------------------------------------------
*/

/*PRINTFLIKE1*/
void test_pso(const char* format_str, ...)
{
    IMPORT int ivi_suppress_test_messages;
    static Bool begining_of_line   = TRUE;
    int        result;
    va_list    ap;
    char       buff[ 100000 ];
    char*      buff_pos;

    if (ivi_suppress_test_messages) { return; }

    va_start(ap, format_str);

    result = ivi_vsprintf(buff, sizeof(buff), format_str, ap);

    va_end(ap);


    if (result == ERROR)
    {
        ivi_print_error();
        set_bug("Error processing TEST error message in test_pso");
        return;
    }

    buff_pos = buff;

    /*CONSTCOND*/
    while (TRUE)
    {
        if (begining_of_line)
        {
            pso("<<TEST>> ");
            begining_of_line = FALSE;
        }

        while (*buff_pos != '\n')
        {
            if (*buff_pos == '\0') break;

            ivi_putc(*buff_pos);
            result++;
            buff_pos++;
        }

        if (*buff_pos == '\0') break;

        ivi_putc(*buff_pos);
        buff_pos++;
        begining_of_line = TRUE;
        result++;

        if (*buff_pos == '\0') break;
    }

    ivi_flush();
}
#endif 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef TEST
/*
 * =============================================================================
 *                                 test_pse
 *
 * Printing to stderr with <<TEST>> prefix
 *
 * This routine writes a formatted string to stdout with each lined prefixed
 * with <<TEST>>. Normally this routine is only used indirectly using the macro
 * TEST_PSE. Other behaviour is similar to p_stderr. Specifically, it is similar
 * to ivi_fprintf with respect to extended formatting options and paging.
 *
 * Macros:
 *    This routine should only be accessed using the macro TEST_PSE which sets
 *    things up so that the printing only occurrs if TEST is defined. If TEST
 *    is not defined, then TEST_PSE becomes nothing (IE, it is removed by the
 *    pre-processor). TEST_PSE is used as follows:
 * |    TEST_PSE(( <format>, arg1, arg2, ....)); // Note DOUBLE parens!
 *
 * Returns:
 *    Return is void, as it is normally used with the TEST_PSE macro.
 *
 * Macros:
 *    TEST_PSE
 *
 * Related:
 *    p_stderr, ivi_fprintf
 *
 * Index: debugging, TEST
 *
 * -----------------------------------------------------------------------------
*/

/*PRINTFLIKE1*/
void test_pse(const char* format_str, ...)
{
    IMPORT int ivi_suppress_test_messages;
    static Bool begining_of_line   = TRUE;
    int        result;
    va_list    ap;
    char       buff[ 100000 ];
    char*      buff_pos;

    if (ivi_suppress_test_messages) { return; }

    va_start(ap, format_str);

    result = ivi_vsprintf(buff, sizeof(buff), format_str, ap);

    va_end(ap);


    if (result == ERROR)
    {
        ivi_print_error();
        set_bug("Error processing TEST error message in test_pse");
        return;
    }

    buff_pos = buff;

    /*CONSTCOND*/
    while (TRUE)
    {
        if (begining_of_line)
        {
            p_stderr("<<TEST>> ");
            begining_of_line = FALSE;
        }

        while (*buff_pos != '\n')
        {
            if (*buff_pos == '\0') break;

            ivi_fputc(stderr, *buff_pos);
            result++;
            buff_pos++;
        }

        if (*buff_pos == '\0') break;

        ivi_fputc(stderr, *buff_pos);
        buff_pos++;
        begining_of_line = TRUE;
        result++;

        if (*buff_pos == '\0') break;
    }

    ivi_fflush(stderr);
}
#endif 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef TEST 
void flag_untested_code(int line_num, const char* file_name)
{ 
    IMPORT int ivi_suppress_test_messages; 
                                           
    if (! ivi_suppress_test_messages) 
    {
        /* We do not use IVI IO because often enough, the relevant code is in IVI IO. */
        fprintf(stderr, "<<TEST>> Executing untested code on line %d of file %s (IL4RT).\n", 
                  line_num, file_name);    
    }
}
#endif  

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef TEST
void flag_suspect_code(int line_num, const char* file_name)
{ 
    IMPORT int ivi_suppress_test_messages; 
                                           
    if (! ivi_suppress_test_messages) 
    {
        /* We do not use IVI IO because often enough, the relevant code is in IVI IO. */
        fprintf(stderr, "<<TEST>> Executing suspect code on line %d of file %s (IL4RT).\n", 
                  line_num, file_name);    
    }
}
#endif 

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef __cplusplus
}
#endif

