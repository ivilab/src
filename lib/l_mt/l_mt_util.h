/*
 * $Id: l_mt_util.h 25612 2020-07-06 22:17:23Z kobus $
 */

#ifndef L_MT_UTIL_H_LIBIVI_INCLUDED
#define L_MT_UTIL_H_LIBIVI_INCLUDED 1

/* Start for c2man */

#include <l/l_sys_def.h>
#include <l/l_sys_io.h>

/* End of includes for c2man */

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


/* End for c2man */

#endif /* L_MT_UTIL_H_LIBIVI_INCLUDED */

