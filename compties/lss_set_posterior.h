/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Jinyan Guan (author).
|
|  Personal and educational use of this code is granted, provided that this
|  header is kept intact, and that the authorship is not misrepresented, that
|  its use is acknowledged in publications, and relevant papers are cited.
|
|  For other use contact the author (Jinyan AT cs DOT arizona DOT edu).
|
|  Please note that the code in this file has not necessarily been adequately
|  tested. Naturally, there is no guarantee of performance, support, or fitness
|  for any particular task. Nonetheless, I am interested in hearing about
|  problems that you encounter.
|
| Authors:
|     Jinyan Guan
|
* =========================================================================== */

/* $Id: lss_set_posterior.h 22561 2019-06-09 00:05:52Z kobus $ */

#ifndef KJB_TIES_LSS_SET_POSTERIOR_H
#define KJB_TIES_LSS_SET_POSTERIOR_H

#include "dbn_cpp/lss_set.h"
#include "dbn_cpp/prior.h"
#include "dbn_cpp/likelihood.h"
#include <vector>

namespace kjb {
namespace ties {

/* \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ */

/*class Person_param_posterior
{
public:
    Person_param_posterior
    (
        const std::vector<Likelihood>& likelihoods,
        const Shared_lss_prior& prior,
        const std::vector<size_t>& person_index_table
    ) : likelihoods_(likelihoods),
        prior_(prior),
        person_index_table_(person_index_table)
    {}

    double operator()(const Lss_set& lsss, size_t d) const
    {
        // need to figure out which person
        const std::vector<std::vector<Vector> >& person_coefs = 
                                    lsss.person_coefs();
        IFT(!person_coefs.empty(), Runtime_error, "Person coefs is empty");
        //size_t person_index = d / person_coefs[0].size();
        //assert(person_index < person_coefs.size());
        assert(d < person_index_table_.size());
        size_t person_index = person_index_table_[d];
        double ll = likelihoods_[person_index](lsss.get_lss_set()[person_index]);
        if(lsss.allow_drift())
        {
            double pr = prior_(lsss, person_index);
            return ll + pr;
        }
        else
        {
            return ll;
        }
    }

private:
    const std::vector<Likelihood>& likelihoods_;
    const Shared_lss_prior& prior_;
    const std::vector<size_t>& person_index_table_;
};
*/


}} // namespace kjb::ties

#endif // KJB_TIES_LSS_SET_POSTERIOR_H

