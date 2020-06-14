/**
 * @file
 * @brief test file to compare convolution methods
 * @author Andrew Predoehl
 */
/*
 * $Id: test_fft.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

/*
 * Approximate time vs. "time factor" input (integer in argv[1]), on my desktop
 * machine:
 *
 * Time factor     Approx time   Remarks
 * -----------------------------------------------------------
 * none specified  3 seconds     Same as time factor of zero
 * 0               3 seconds     quick test, not very thorough
 * 1               3 minutes     much more thorough test
 */

#include <l/l_sys_rand.h>
#include <m/m_convolve.h>
#include <m2/m2_ncc.h>
#include <m_cpp/m_convolve.h>
#include <i_cpp/i_matrix.h>
#include <l_cpp/l_test.h>

namespace
{

double NOISE_TOL = 1e-6; // this is a conservative value; could be 1e-10.


/**
 * @brief test correcteness of class Fftw_convolution_2d, of variable sizes.
 *
 * @param S1 size (edge length) of square matrix as one convolutional argument
 * @param S2 size (edge length) of square matrix of other convo arg, the filter
 * @param take_your_time if false, abbreviate the test, namely, skip the slow
 *                       direct convolution step.  It's a better test if true.
 * @return ivi_c::ERROR or ivi_c::NO_ERROR as appropriate
 *
 * The class Fftw_convolution_2d emulates the behavior of two C-library 
 * functions (which do not emulate each other -- there are slight differences).
 * The two C-library functions are convolve_matrix() and
 * fourier_convolve_matrix().  The slight differences between them are in the
 * way they handle boundary conditions.  This function emulates each in turn
 * with matrices of random content, and tests that the results are equal, to
 * within tiny results due to numerical noise.  If not, the program aborts.
 *
 * @bug The noise tolerance is fixed by NOISE_TOL even though it probably
 * should vary by S1*S1*S2*S2.
 */
int trial(size_t S1, size_t S2, bool take_your_time=true)
{
    const ivi::Matrix   m1(ivi::create_random_matrix(S1, S1)),  // "data"
                        m2(ivi::create_random_matrix(S2, S2));  // "filter"
    ivi::Matrix m_cpp_fft_reflect, m_cpp_fft_zeropad;

    // fancy fft convolution
    ivi::Fftw_convolution_2d convo(S1, S1, S2, S2);
    convo.set_mask(m2);
    convo.reflect_and_convolve(m1, m_cpp_fft_reflect);
    convo.convolve(m1, m_cpp_fft_zeropad);

    // C lib direct convolution with reflection
    double dr = 0;
    if (take_your_time)
    {
        ivi_c::Matrix *cm4 = 00;
        IVI(ERE(convolve_matrix(&cm4, m1.get_c_matrix(), m2.get_c_matrix())));
        ivi::Matrix m4(cm4);
        dr = ivi::max_abs_difference(m4, m_cpp_fft_reflect);
    }

    // C lib FFT convolution with zeropad
    ivi_c::Matrix *cm5 = 00;
    IVI(ERE(fourier_convolve_matrix(&cm5,
                                    m1.get_c_matrix(), m2.get_c_matrix())));
    ivi::Matrix m5(cm5);
    const double dz = ivi::max_abs_difference(m5, m_cpp_fft_zeropad);

    if (ivi_c::is_interactive())
    {
        IVI(TEST_PSE((  "max abs difference, reflection = %e\n"
                        "max abs difference, zero pad = %e\n", dr, dz)));
    }

    TEST_TRUE(dr < NOISE_TOL);   // error in reflect_and_convolve() method
    TEST_TRUE(dz < NOISE_TOL);   // error in convolve() method

    return ivi_c::NO_ERROR;
}

}

int main(int argc, char** argv)
{
    int time = 0;

    IVI(EPETE(scan_time_factor(argv[1], &time)));

    /* The even/odd combinations here have shaken out bugs previously, but I
     * think that has to do more with the prime/composite variation than the
     * parity variation.  TBH one should thoroughly test with a wide variety
     * of sizes (such that the sums of the sizes have a wide variety of
     * factorizations).
     */
    try 
    {
        // basic, quick test (not as effective)
        if (0 == time) IVI(EPETE(trial(500, 100, 0)));

        // better quality test, about 60x slower.
        for( ; time > 0; --time)
        {
            IVI(EPETE(trial(500, 100)));
            IVI(EPETE(trial(501, 100)));
            IVI(EPETE(trial(500, 101)));
            IVI(EPETE(trial(501, 101)));
        }
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }

    RETURN_VICTORIOUSLY();
}

