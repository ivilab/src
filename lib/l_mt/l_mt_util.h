/*
 * $Id: l_mt_util.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef L_MT_UTIL_H_LIBIVI_INCLUDED
#define L_MT_UTIL_H_LIBIVI_INCLUDED 1

#include <l/l_sys_def.h>
#include <l/l_sys_io.h>

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

void *ivi_mt_malloc(Malloc_size num_bytes);

void ivi_mt_free(void *ptr);

FILE *ivi_mt_fopen(
    const char *input_fd_name,
    const char *mode
);

long ivi_mt_fread(
    FILE *fp,
    void *buff,
    size_t len
);

long ivi_mt_fwrite(
    FILE *fp,
    const void *line,
    size_t len
);

long ivi_mt_fprintf(
    FILE* fp,
    const char* format_str,
    ...
);

int ivi_mt_fclose(FILE *fp);


/* below:  end-of-file boilerplate */

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif


#endif /* L_MT_UTIL_H_LIBIVI_INCLUDED */

