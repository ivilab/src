
/* $Id: l_error.c 25612 2020-07-06 22:17:23Z kobus $ */

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
* =========================================================================== */

#include "l/l_gen.h"     /* Only safe as first include in a ".c" file. */
#include "l/l_error.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

/* =============================================================================
 *                               check_num_args
 *
 * Checks the number of program arguments
 *
 * This routine checks the number of program arguments, and if they are not
 * within the bounds set by the parameters, then it prints and error message to
 * that effect and exits.
 *
 * Note: The ranges are in terms of user arugments which is one less than the
 * value of argc. The first argument is usually the program name, and we don't
 * count that here.
 *
 * Index: error handling
 *
 * -----------------------------------------------------------------------------
*/

void check_num_args(int argc,
                    int min_num_args_allowed,
                    int max_num_args_allowed,
                    const char* usage)
{


    argc--;

    if (    (usage != NULL)
         && (    ((min_num_args_allowed > 0) && (argc < min_num_args_allowed))
              || ((max_num_args_allowed > 0) && (argc > max_num_args_allowed))
            )
       )
    {
        ivi_fprintf(stderr, "Usage:\n  ");
        ivi_fprintf(stderr, usage);
        ivi_fprintf(stderr, "\n\n");
        ivi_exit(EXIT_FAILURE);
    }
    else if (    (min_num_args_allowed > 0)
              && (max_num_args_allowed > 0)
              && (min_num_args_allowed == max_num_args_allowed)
              && (argc != min_num_args_allowed)
            )
    {
        ivi_fprintf(stderr, "Exactly %d program argument is required.\n",
                    min_num_args_allowed);
        ivi_exit(EXIT_FAILURE);
    }
    else if ((min_num_args_allowed > 0) && (argc < min_num_args_allowed))
    {
        ivi_fprintf(stderr, "At least %d program arguments are required.\n",
                    min_num_args_allowed);
        ivi_exit(EXIT_FAILURE);
    }
    else if ((max_num_args_allowed > 0) && (argc > max_num_args_allowed))
    {
        ivi_fprintf(stderr, "At most %d program arguments can be used.\n",
                    max_num_args_allowed);
        ivi_exit(EXIT_FAILURE);
    }
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef __cplusplus
}
#endif

