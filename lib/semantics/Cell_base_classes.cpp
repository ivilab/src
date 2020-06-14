/* $Id: Cell_base_classes.cpp 25499 2020-06-14 13:26:04Z kobus $ */

/*!
 * @file Cell_base_classes.cpp
 *
 * @author Colin Dawson 
 */


#include <semantics/Cell_base_classes.h>
#include <prob_cpp/prob_distribution.h>
#include <prob_cpp/prob_sample.h>
#include <cmath>

namespace semantics
{
    //// Gamma hyperparameters for concentration parameter alpha
    const double& Context_cell_base::a() {static double a = 1.0; return a;}
    const double& Context_cell_base::b() {static double b = 1.0; return b;}
    
    void Context_cell_base::resample_alpha()
    {
        if(count_ == 0)
        {
            /// if the count is 0, this is a test item in a novel context,
            /// so we have no data to inform alpha.  Therefore, just sample from
            /// the prior.
            ivi::Gamma_distribution gamma(a(), b());
            alpha_ = ivi::sample(gamma);
            return;
        }
        // std::cerr << "Resampling w. Old value is " << w_ << std::endl;
        // std::cerr << "Count is " << count_ << std::endl;
        ivi::Beta_distribution beta(alpha_ + 1, count_);
        w_ = ivi::sample(beta);
        // std::cerr << "New value is " << w_ << std::endl;

        // std::cerr << "Resampling s. Old value is " << s_ << std::endl;
        ivi::Bernoulli_distribution bern(count_ / (count_ + alpha_));
        s_ = ivi::sample(bern);
        // std::cerr << "New value is " << s_ << std::endl;

        // std::cerr << "Resampling alpha. Old value is " << alpha_ << std::endl;
        // std::cerr << "Table count is " << count_ << std::endl;
        ivi::Gamma_distribution gamma(a() + table_count_, b() - log(w_));
        alpha_ = ivi::sample(gamma);
        // std::cerr << "New value is " << alpha_ << std::endl;
    }
}
