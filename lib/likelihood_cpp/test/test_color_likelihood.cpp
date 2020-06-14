/* $Id: test_color_likelihood.cpp 25499 2020-06-14 13:26:04Z kobus $ */
#include <likelihood_cpp/color_likelihood.h>
#include <iostream>
#include <map>
#include <i_cpp/i_image.h>
#include <l/l_io.h>

using namespace ivi;
using namespace std;

int main(int argc, char **argv)
{
    const int SIZE = 100;

    Image I(static_cast<int>(SIZE), static_cast<int>(SIZE));
    for(int i = 0; i < I.get_num_rows(); i++)
    {
        for(int j = 0; j < I.get_num_cols(); j++)
        {
            if(i < SIZE / 2 && j < SIZE / 2)
            {
                I(i, j).r = 0.0;
                I(i, j).g = 0.0;
                I(i, j).b = 0.0;
            }
            else if(i >= SIZE / 2 && j >= SIZE / 2)
            {
                I(i, j).r = 0.0;
                I(i, j).g = 0.0;
                I(i, j).b = 0.0;
            }
            else
            {
                I(i, j).r = 255.0;
                I(i, j).g = 255.0;
                I(i, j).b = 255.0;
            }
        }
    }

    Int_matrix mask(SIZE, SIZE);
    for(int i = 0; i < mask.get_num_rows(); i++)
    {
        for(int j = 0; j < mask.get_num_cols(); j++)
        {
            if(i < SIZE / 4 && j < SIZE / 4)
            {
                mask(i, j) = 1;
                I(i, j).r = 255.0;
                I(i, j).g = 0.0;
                I(i, j).b = 0.0;
            }
            else if(i >= SIZE / 4 && j >= 3 * SIZE / 4)
            {
                mask(i, j) = 2;
                I(i, j).r = 255.0;
                I(i, j).g = 0.0;
                I(i, j).b = 0.0;
            }
            else
            {
                mask(i, j) = 0;
            }
        }
    }

    I.display();

    Color_likelihood clh(I);
    map<int, Region_vector> regions = get_regions_from_mask(mask, 2);
    cout << "likelihood: " << clh(regions) << endl;

    ivi_c::prompt_to_continue();

    return EXIT_SUCCESS;
}

