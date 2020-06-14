/**
 * @file
 * @brief Unit test for GSL sampling a discrete empirical distribution
 * @author Andrew Predoehl
 */
/*
 * $Id: test_randist.cpp 25499 2020-06-14 13:26:04Z kobus $
 */


#include <l/l_incl.h>
#include <gsl_cpp/gsl_randist.h> 
#include <vector>
#include <numeric>
#include "test_expects.h"

int main()
{
    const int TSZ = 100000;
    const int BINS = 3;
    std::vector< int > bins( BINS, 0 );
    const double weights[ BINS ] = { 1, 2, 3.5 };
    const ivi::Vector wts( BINS, weights );
    double sum = std::accumulate( weights, weights + BINS, 0.0 );

    ivi::Gsl_rng_default rng;
    rng.seed( 12345ul ); 

    ivi::Gsl_ran_discrete disc( wts );

    for( int iii = 0; iii < TSZ; ++iii )
        bins.at( disc.sample( rng ) ) += 1;

    // The number of samples in each bin is a binomial random variable
    for( int iii = 0; iii < BINS; ++iii ) {
        double  mean = weights[ iii ] / sum,                // b.r.v. mean
                sigma = sqrt( mean * ( 1 - mean ) * TSZ ),  // b.r.v. sigma
                z = ( bins.at( iii ) - TSZ * mean ) / sigma;
        if ( ivi_c::is_interactive() )
            printf("mean=%f\tsigma=%f\tz-score=%f\n", mean, sigma, z );
        we_expect_that( fabs( z ) < 2.0 );
    }

    return EXIT_SUCCESS;
}

