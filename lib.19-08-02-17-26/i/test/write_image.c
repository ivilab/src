/* $Id$ */
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
   |  Author:  Kyle Simek
 * =========================================================================== */

#include <m/m_incl.h>
#include <l/l_incl.h>
#include <i/i_float.h>
#include <i/i_float_io.h>

int main (int argc, char *argv[])
{
    KJB_image* ip = NULL;
    const int NUM_ROWS = 256;
    const int NUM_COLS = 256;
    int row, col;

    kjb_init();

    EPETE(get_target_image(&ip, NUM_ROWS, NUM_COLS));

    EPETE(kjb_write_image(ip, "out.png"));

    
    return EXIT_SUCCESS;
}