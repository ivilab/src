/**
 * @file
 * @brief Functions and structures specific to file and memory pool indexing.
 * @author Scott Morris
 * @author Alan Morris
 *
 * Originally from TopoFusion.
 */
/*
 * $Id: index.h 25612 2020-07-06 22:17:23Z kobus $
 *
 * Recommended tab width:  4
 */

#ifndef INDEX_H_INCLUDED_UOFARIZONAVISION
#define INDEX_H_INCLUDED_UOFARIZONAVISION

/* Start for c2man */

#include "l/l_sys_def.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
namespace TopoFusion {
#endif
#endif

int initIndexFile( void );

void closeIndexFile( void );

int addIndexEntry( int, int, char, char, const char*, int );

int findTileOnDisk( int x, int y, int typ, char zone );

int getTileFromDisk( int x, int y, int typ, char zone, char *buf, size_t );

int invalidateEntry( int x, int y, int typ, char zone );

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
} /* namespace TopoFusion */
} /* namespace ivi_c */
#endif
} /* extern "C" */
#endif


/* End for c2man */

#endif  /* INDEX_H_INCLUDED_UOFARIZONAVISION */

