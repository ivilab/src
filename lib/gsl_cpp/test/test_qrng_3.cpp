/**
 * @file
 * @author Andrew Predoehl
 * @brief demonstrate static typing of Gsl_Qrng_basic prevents messes.
 */

/* $Id: test_qrng_3.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_incl.h>
#include <gsl_cpp/gsl_qrng.h>


/*
 * The purpose of this test is mostly to make sure that my template hackery
 * did the job:  it prevents you from assigning qrngs of different types.
 */
int main()
{
    ivi::Gsl_Qrng_Sobol qs( 10 );
    ivi::Gsl_Qrng_Niederreiter qn( 10 ), qm( qn );
    ivi::Gsl_Qrng_Sobol qs2( 15 );

    /*
    ivi::Gsl_Qrng_Niederreiter qp( qs ); //   <------ that should not compile
    */

    qn = qm;

    qs = qs2;

    /*
    qn = qs;    //       <-----------  that should not compile
    */

    /*
    ivi::Gsl_Qrng_basic *r1 = &qs, *r2 = &qn;   //   <--- also no way
    */

    return EXIT_SUCCESS;
}
