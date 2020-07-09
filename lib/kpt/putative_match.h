#ifndef LIB_PUTATIVE_MATCH_H_
#define LIB_PUTATIVE_MATCH_H_

/* Start for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

#include "kpt/keypoint.h"

/* End of includes for c2man */

int get_putative_matches
(
    const Keypoint_vector *x_kvp,
    const Keypoint_vector *y_kvp,
    const double          strength_threshold,
    const double          redundance_threshold,
    int                   *num_putative_matches,
    Int_matrix            **match_idx_impp
);

int get_putative_matches_2
(
    const Keypoint_vector *target_kvp,
    const Keypoint_vector *candidate_kvp,
    const double          strength_threshold,
    const double          redundance_threshold,
    int                   *num_putative_matches,
    Int_matrix            **match_idx_impp
);

int get_putative_matches_3
(
    const Keypoint_vector *target_kvp,
    const Keypoint_vector *candidate_kvp,
    const double          strength_threshold,
    const double          location_threshold,
    int                   *num_putative_matches,
    Int_matrix            **match_idx_impp
);

int extract_match_positions
(
    Matrix                    **target_pos_mpp,
    Matrix                    **candidate_pos_mpp,
    const Keypoint_vector     *target_kvp,
    const Keypoint_vector     *candidate_kvp,
    const Int_matrix          *mask_imp,
    const int                 num_matches
);
 
int extract_match_positions_1
(
    Matrix                    **target_pos_mpp,
    Matrix                    **candidate_pos_mpp,
    const Keypoint_vector     *target_kvp,
    const Keypoint_vector     *candidate_kvp,
    const Int_matrix          *mask_imp,
    const int                 num_matches,
    Int_matrix                **kpt_idx_impp
);
 
int extract_match_positions_2
(
    Matrix                    **target_pos_mpp,
    Matrix                    **candidate_pos_mpp,
    const Keypoint_vector     *target_kvp,
    const Keypoint_vector     *candidate_kvp,
    const Int_vector          *mask_ivp,
    const int                 num_matches,
    Int_vector                **kpt_to_inlier_ivpp,
    Int_vector                **inlier_to_kpt_ivpp
);

int get_constrained_keypoint_match 
(
    const Keypoint        *target_kpt,
    const Keypoint_vector *candidate_kvp,
    const double          dist_ratio,
    const double          dist_radius
);

int print_keypoint_parameters
(
    const Keypoint_vector     *target_kvp,
    const Keypoint_vector     *candidate_kvp,
    const Int_matrix          *mask_imp,
    const int                 num_matches
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif /* LIB_PUTATIVE_MATCH_H_ */
