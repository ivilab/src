/**
 * @file
 * @brief test prog for Fftw_convolution_2d
 * @author Andrew Predoehl
 *
 * If this fails, try the interactive version (same name) to see what is
 * happening.
 */
/*
 * $Id: test_reflection.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l_cpp/l_test.h>
#include <m_cpp/m_convolve.h>
#include <iostream>
#include <l_cpp/l_util.h>

namespace
{

int test(const ivi::Matrix& x, const ivi::Matrix& y, const ivi::Matrix& a)
{
    ivi::Fftw_convolution_2d::Sizes s(
            x.get_num_rows(), x.get_num_cols(),
            y.get_num_rows(), y.get_num_cols());

    ivi::Matrix z;

    IVI(ERE(ivi::debug::test_reflect_into_input_buf(x, &z, s)));

    if (z == a)
    {
        return ivi_c::NO_ERROR;
    }

    std::cout
        << "input:\n" << x
        << "\nactual output:\n" << z
        << "\nexpected output:\n" << a << '\n';

    return ivi_c::ERROR;
}

}

int main(int argc, char** argv)
{
    const double d[] = {-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7};
    ivi::Matrix x(4, 4, d); 

    IVI(EPETE(test(x, ivi::Matrix(1, 1), x)));

    const double t1[] = {
        -8.0, -7.0, -6.0, -5.0, -5.0, -8.0,
        -4.0, -3.0, -2.0, -1.0, -1.0, -4.0,
         0.0,  1.0,  2.0,  3.0,  3.0,  0.0,
         4.0,  5.0,  6.0,  7.0,  7.0,  4.0,
         4.0,  5.0,  6.0,  7.0,  7.0,  4.0,
        -8.0, -7.0, -6.0, -5.0, -5.0, -8.0
    };
    IVI(EPETE(test(x, ivi::Matrix(3, 3),
                    ivi::Matrix(6, 6, t1)
        )));

    const double t2[] = {
        -8.0, -7.0, -6.0, -5.0, -5.0, -6.0, -7.0, -8.0,
        -4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0,
         0.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  0.0,
         4.0,  5.0,  6.0,  7.0,  7.0,  6.0,  5.0,  4.0,
         4.0,  5.0,  6.0,  7.0,  7.0,  6.0,  5.0,  4.0,
         0.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  0.0,
        -4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0,
        -8.0, -7.0, -6.0, -5.0, -5.0, -6.0, -7.0, -8.0
    };
    IVI(EPETE(test(x, ivi::Matrix(5, 5),
                    ivi::Matrix(8,8,t2)
        )));

    const double t3[] = {
        -8.0, -7.0, -6.0, -5.0, -5.0, -6.0, -8.0,
        -4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -4.0,
         0.0,  1.0,  2.0,  3.0,  3.0,  2.0,  0.0,
         4.0,  5.0,  6.0,  7.0,  7.0,  6.0,  4.0
    };
    IVI(EPETE(test(x, ivi::Matrix(1, 4),
                    ivi::Matrix(4,7,t3)
        )));

    const double t4[] = {
        -8.0, -7.0, -6.0, -5.0,
        -4.0, -3.0, -2.0, -1.0,
         0.0,  1.0,  2.0,  3.0,
         4.0,  5.0,  6.0,  7.0,
         4.0,  5.0,  6.0,  7.0,
         0.0,  1.0,  2.0,  3.0,
        -8.0, -7.0, -6.0, -5.0
    };
    IVI(EPETE(test(x, ivi::Matrix(4, 1),
                    ivi::Matrix(7,4,t4)
        )));

    const double t5[] = {
        -8.0, -7.0, -6.0, -5.0, -5.0,
        -4.0, -3.0, -2.0, -1.0, -1.0,
         0.0,  1.0,  2.0,  3.0,  3.0,
         4.0,  5.0,  6.0,  7.0,  7.0
    };
    IVI(EPETE(test(x, ivi::Matrix(1, 2),
                    ivi::Matrix(4,5,t5)
        )));

    const double t6[] = {
        -8.0, -7.0, -6.0, -5.0,
        -4.0, -3.0, -2.0, -1.0,
         0.0,  1.0,  2.0,  3.0,
         4.0,  5.0,  6.0,  7.0,
         4.0,  5.0,  6.0,  7.0
    };
    IVI(EPETE(test(x, ivi::Matrix(2, 1),
                    ivi::Matrix(5,4,t6)
        )));

    RETURN_VICTORIOUSLY();
}
