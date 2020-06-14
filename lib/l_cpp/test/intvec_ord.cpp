/**
 * @file
 * @author Andrew Predoehl
 * @brief test the order operators <, <=, >, >= for Int_vector.
 */

/*
 * $Id: intvec_ord.cpp 25499 2020-06-14 13:26:04Z kobus $
 */
#include <l_cpp/l_int_vector.h>

#define BAD bad( __LINE__ )

int bad( int line )
{
    ivi_c::p_stderr( "Failure on line %d.\n", line );
    return EXIT_FAILURE;
}

int main()
{
    const int   v1[] = { 56, 67, 78, 89 },
                v2[] = { 56, 99 },
                v3[] = { 56, 67, 78 };

    const ivi::Int_vector   w1( 4, v1 ),
                            w2( 2, v2 ),
                            w3( 3, v3 );

    if ( w1 < w1 )  return BAD;
    if ( w2 < w2 )  return BAD;
    if ( w3 < w3 )  return BAD;
    if ( w1 > w1 )  return BAD;
    if ( w2 > w2 )  return BAD;
    if ( w3 > w3 )  return BAD;

    if ( w2 < w1 )  return BAD; // similar to "apple" < "axe"
    if ( w1 > w2 )  return BAD;
    if ( w2 < w3 )  return BAD;
    if ( w3 > w2 )  return BAD;

    if ( w1 < w3 )  return BAD; // similar to "app" < "apple"
    if ( w3 > w1 )  return BAD;

    // similar to the above, but allowing for equality

    if ( w2 <= w1 ) return BAD; // similar to "apple" <= "axe"
    if ( w1 >= w2 ) return BAD;
    if ( w2 <= w3 ) return BAD;
    if ( w3 >= w2 ) return BAD;

    if ( w1 <= w3 ) return BAD; // similar to "app" <= "apple"
    if ( w3 >= w1 ) return BAD;

    return EXIT_SUCCESS;
}
