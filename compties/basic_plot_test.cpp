/**
 * @file
 * @brief this is just a basic demo of the plotting functions.
 *
 * It is only used for the iPlant Discovery Environment integration.
 */
/*
 * $Id: basic_plot_test.cpp 20885 2016-10-12 02:20:28Z jguan1 $
 */

#include <l/l_sys_def.h>
#include <p/p_plot.h>
#include <l_cpp/l_util.h>
#include <m_cpp/m_vector.h>

int main(int argc, const char** argv)
{
    const size_t N=100;
    kjb::Vector t(N), y1(N), y2(N);
    for (size_t i = 0; i < N; ++i)
    {
        t[i] = 0.1 * i;
        y1[i] = sin(0.5 * i) + 0.3 * kjb_c::kjb_rand();
        y2[i] = cos(0.5 * i) + 0.3 * kjb_c::kjb_rand();
    }

    const int pd = kjb_c::plot_open();
    KJB(EPETE(plot_curve(pd, t.get_c_vector(), y1.get_c_vector(), "sinus")));
    KJB(EPETE(plot_curve(pd, t.get_c_vector(), y2.get_c_vector(), "cosignatory")));
    KJB(EPETE(plot_set_x_legend(pd, "Time step (see input file)")));
    KJB(EPETE(plot_set_y_legend(pd, "Normalized observable level")));
    KJB(EPETE(plot_set_title(pd, "Observable data and Coupled oscillator fit", 0, 0)));
    KJB(EPETE(save_plot(pd, "whisk.eps")));
    KJB(EPETE(plot_close(pd)));

    return EXIT_SUCCESS;
}
