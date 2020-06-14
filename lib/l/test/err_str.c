/**
 * @file
 * @author Andrew Predoehl
 */

/* $Id: err_str.c 25499 2020-06-14 13:26:04Z kobus $
 */

#include "l/l_incl.h"

#define FAIL()  fail( __LINE__ )

#define TENDIGITS "0123456789"

#define TENSZ 10

#define BUFSZ 30 

static int fail( int line )
{
    ivi_fprintf( stderr, "failure on line %d\n", line );
    return EXIT_BUG;
}

int main (void)
{
    int count;
    const char *ten = TENDIGITS;
    char buf[ BUFSZ ];

    ASSERT( TENSZ == signed_strlen( TENDIGITS ) );
    ASSERT( BUFSZ > 2 * ( 1 + TENSZ ) );

    EPETE( ivi_init() );

    set_error( ten );

    count = ivi_get_strlen_error();

    if ( count != TENSZ )
        return FAIL();

    ivi_get_error( buf, BUFSZ );
    if ( ! STRNCMP_EQ( buf, ten, BUFSZ ) )
        return FAIL();

    count = ivi_get_strlen_error();

    if ( count != 0 )
        return FAIL();

    ivi_get_error( buf, BUFSZ );

    if ( buf[ 0 ] != 0 )
        return FAIL();

    set_error( ten );
    add_error( ten );
    count = ivi_get_strlen_error();

    if ( count != 2 * TENSZ + 1 )
        return FAIL();

    ivi_get_error( buf, BUFSZ );

    if ( ! STRNCMP_EQ( buf, TENDIGITS "\n" TENDIGITS , BUFSZ ) )
        return FAIL();

    ivi_cleanup();

    return EXIT_SUCCESS;
}
