/**
 * @file
 * @brief this is just a basic demo of the plotting functions.
 *
 * It is only used for the iPlant Discovery Environment integration.
 */
/*
 * $Id: basic_plot_test.cpp 25797 2020-09-19 21:14:43Z kobus $
 */

#include <l/l_sys_def.h>
#include <p/p_plot.h>
#include <l_cpp/l_util.h>
#include <m_cpp/m_vector.h>

int main(int argc, const char** argv)
{
    const size_t N=100;
    ivi::Vector t(N), y1(N), y2(N);
    for (size_t i = 0; i < N; ++i)
    {
        t[i] = 0.1 * i;
        y1[i] = sin(0.5 * i) + 0.3 * ivi_c::ivi_rand();
        y2[i] = cos(0.5 * i) + 0.3 * ivi_c::ivi_rand();
    }

    const int pd = ivi_c::plot_open();
    IVI(EPETE(plot_curve(pd, t.get_c_vector(), y1.get_c_vector(), "sinus")));
    IVI(EPETE(plot_curve(pd, t.get_c_vector(), y2.get_c_vector(), "cosignatory")));
    IVI(EPETE(plot_set_x_legend(pd, "Time step (see input file)")));
    IVI(EPETE(plot_set_y_legend(pd, "Normalized observable level")));
    IVI(EPETE(plot_set_title(pd, "Observable data and Coupled oscillator fit", 0, 0)));
    IVI(EPETE(save_plot(pd, "whisk.eps")));
    IVI(EPETE(plot_close(pd)));

    return EXIT_SUCCESS;
}
