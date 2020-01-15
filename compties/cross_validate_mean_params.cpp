/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author).
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
| Authors:
|     Jinyan Guan
|
* =========================================================================== */

/* $Id: cross_validate_evaluate.cpp 20194 2015-12-19 21:51:31Z predoehl $ */

#ifndef KJB_HAVE_ERGO
#error "You need libergo to use this program"
#endif

#include <l_cpp/l_filesystem.h>

#include "dbn_cpp/data.h"
#include "dbn_cpp/likelihood.h"
#include "dbn_cpp/gradient.h"
#include "dbn_cpp/linear_state_space.h"
#include "dbn_cpp/lss_set_sampler.h"
#include "dbn_cpp/options.h"
#include "dbn_cpp/experiment.h"
#include "dbn_cpp/util.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>


using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
#ifdef TEST
    kjb_c::kjb_init();
    kjb_c::kjb_l_set("heap-checking", "off");
    kjb_c::kjb_l_set("initialization-checking", "off");
#endif
    Ties_experiment exp = experiment_from_cl_options(argc, argv);

    // set model name
    if(exp.run.fit_ind_clo)
    {
        exp.model_name = "MLE-CLO";
    }
    else if(exp.prior.fixed_clo)
    {
        exp.model_name = "Shared-CLO";
    }
    else
    {
        exp.model_name = "MAP-CLO";
    }
   
    string fold_fmt_str(exp.out_dp + "/fold-%02d/training/");
    string list_fmt_str(exp.out_dp + "/fold-%02d/training-list.txt");
    boost::format fold_fmt(fold_fmt_str);
    boost::format list_fmt(list_fmt_str);
    std::vector<std::string> fold_dirs = dir_names_from_format(fold_fmt_str);
    size_t K = fold_dirs.size();

    std::string err_dp = exp.out_dp + "/errors/";
    ETX(kjb_c::kjb_mkdir(err_dp.c_str()));

    // for all the observables 
    const std::vector<std::string>& obs_names = exp.likelihood.obs_names;
    size_t num_obs = obs_names.size();
    size_t num_params = param_length(exp.lss.num_oscillators);
    std::vector<Vector> pred_coef_aves(num_params);
    int num_poly_params = exp.lss.polynomial_degree + 1;
    std::vector<std::vector<Vector> > poly_pred_aves(exp.lss.num_oscillators,
            std::vector<Vector>(num_poly_params));
    std::vector<Vector> poly_vars_aves(exp.lss.num_oscillators);
    std::vector<double> variances(num_params, 0.0);
    Vector obs_noise_aves((int)num_obs, 0.0);
    for(size_t k = 1; k <= K; k++)
    {
        Ties_experiment exp_copy = exp;
        string in_dir = (fold_fmt % k).str();
        exp_copy.run.in_dp = in_dir;
        exp_copy.run.read_model = true;
        string training_fp = (list_fmt % k).str();
        exp_copy.data.id_list_fp.push_back(training_fp);
        Lss_set_sampler lss_set_sampler(exp_copy);
        Lss_set lss_set = lss_set_sampler.lss_set();
        const std::vector<Vector>& pred_coefs = lss_set.pred_coefs();
        const std::vector<double>& vars = lss_set.variances();
        const Vector& obs_sigmas = lss_set.get_noise_sigmas();
        assert(variances.size() == num_params);
        assert(pred_coefs.size() == vars.size());

        for(size_t i = 0; i < pred_coefs.size(); i++)
        {
            if(k == 1)
            {
                pred_coef_aves[i] = pred_coefs[i];
            }
            else
            {
                pred_coef_aves[i] += pred_coefs[i];
            }
            variances[i] += vars[i];
        }

        obs_noise_aves += obs_sigmas;
    }

    std::for_each(pred_coef_aves.begin(), pred_coef_aves.end(), 
                  boost::bind(&Vector::divide,  _1, (double)K));
    std::for_each(variances.begin(), variances.end(), 
                  boost::bind(std::divides<double>(), _1, K)); 
    obs_noise_aves /= K;
    string param_fp(exp.out_dp + "/params.txt");
    ofstream param_ofs(param_fp.c_str());
    IFTD(param_ofs.is_open(), IO_error, "can't open file %s", (param_fp.c_str()));
    for(size_t i = 0; i < pred_coef_aves.size(); i++)
    {
        param_ofs << pred_coef_aves[i] << std::endl;
    }
    std::setprecision(10);
    std::copy(variances.begin(), variances.end(), ostream_iterator<double>(param_ofs, " "));
    string obs_noise_fp(exp.out_dp + "/obs_noise_sigmas.txt");
    ofstream obs_noise_ofs(obs_noise_fp.c_str());
    IFTD(obs_noise_ofs.is_open(), IO_error, "can't open file %s", (obs_noise_fp.c_str()));
    obs_noise_ofs << obs_noise_aves << std::endl;
    
    return EXIT_SUCCESS;
}

