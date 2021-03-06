/* =========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
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
   |  Author:  Ernesto Brau
 * =========================================================================== */

#include <gp_cpp/gp_base.h>
#include <gp_cpp/gp_mean.h>
#include <gp_cpp/gp_covariance.h>
#include <gp_cpp/gp_likelihood.h>
#include <gp_cpp/gp_normal.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <m_cpp/m_vector.h>
#include <m_cpp/m_matrix.h>
#include <l_cpp/l_test.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

using namespace std;
using namespace ivi;

const bool VERBOSE = true;

typedef gp::Predictive<
                gp::Manual,
                gp::Squared_exponential,
                gp::Linear_gaussian> Pred;

int main(int, char**)
{
    const size_t N = 100;
    double sg = 0.1;

    // training data
    gp::Inputs X(N);
    Vector f(N);
    Vector y(N);
    for(size_t n = 0; n < N; n++)
    {
        X[n].set(n);
        f[n] = n;
        y[n] = f[n] + sample(Normal_distribution(0.0, sg));
    }

    // (single) test input
    Vector x_s = Vector().set(N);

    // create predictive
    Vector mu = create_random_vector(N + 1);
    gp::Manual m1(X, Vector(mu.begin(), mu.end() - 1));
    m1.add(x_s, mu[N]);
    gp::Squared_exponential sqex(1.0, 1.0);

    Pred pred = gp::make_predictive(m1, sqex, sg, X, y, gp::Inputs(1, x_s));
    const MV_normal_distribution& P = pred.normal();

    ////// test single-test mean
    double eps = 1e-5;

    // test size
    TEST_TRUE(P.get_dimension() == 1);

    // compute it specifically for a single test input
    Vector k_s(N);
    transform(
        X.begin(),
        X.end(),
        k_s.begin(),
        boost::bind<double>(boost::ref(sqex), x_s, _1));
    Matrix K = apply_cf(sqex, X.begin(), X.end())
                + create_diagonal_matrix(N, sg*sg);
    Vector m(mu.begin(), mu.end() - 1);
    double mu_s = mu[N] + dot(k_s, matrix_inverse(K) * (y - m));

    // compare with Pred's, which is computed in general
    if(VERBOSE) cout << P.get_mean()[0] << " == " << mu_s << endl;
    TEST_TRUE(fabs(P.get_mean()[0] - mu_s) < eps);

    ////// test single-test variance
    // compute it specifically for a single test input
    double k_ss = sqex(x_s, x_s);
    double v_s = k_ss - dot(k_s, matrix_inverse(K) * k_s);

    // compare with Pred's, which is computed in general
    if(VERBOSE) cout << P.get_covariance_matrix()(0, 0) << " == " << v_s << endl;
    TEST_TRUE(fabs(P.get_covariance_matrix()(0, 0) - v_s) < eps);

    RETURN_VICTORIOUSLY();
}

