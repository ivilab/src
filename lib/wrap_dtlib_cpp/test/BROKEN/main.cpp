#include <wrap_dtlib_cpp/texture.h>
#include <wrap_dtlib_cpp/textonhisto.h>
#include <iostream>

using namespace DTLib;
using namespace ivi;
int main(int argc, char ** argv)
{
    std::string img_name("~/data/furniture/ucbroom/");
    img_name.append(argv[1]);
    img_name.append(".jpg");
    std::string seg_name("~/data/furniture/ucbroom/geometric_context/");
    seg_name.append(argv[1]);
    seg_name.append("_imsegs");
    std::string out_oem_name(argv[2]);
    out_oem_name.append("/");
    out_oem_name.append(argv[1]);
    std::string out_oev_name(out_oem_name);
    std::string out_dm_name(out_oem_name);
    std::string out_dv_name(out_oem_name);
    out_oem_name.append("_oe_mean.txt");
    out_oev_name.append("_oe_var.txt");
    out_dm_name.append("_dog_mean.txt");
    out_dv_name.append("_dog_var.txt");

    std::string in_oem_name(argv[3]);
    in_oem_name.append("/");
    in_oem_name.append(argv[1]);
    std::string in_oev_name(in_oem_name);
    std::string in_dm_name(in_oem_name);
    std::string in_dv_name(in_oem_name);
    in_oem_name.append("_OE_mean.out");
    in_oev_name.append("_OE_variance.out");
    in_dm_name.append("_DOG_mean.out");
    in_dv_name.append("_DOG_variance.out");

    ivi::Image img(img_name.c_str());
    ivi::Matrix m(seg_name.c_str());
    ivi::Int_matrix im = create_int_matrix_from_matrix_floor(m);

    ivi::Matrix Oe_mean;
    ivi::Matrix Oe_var;
    ivi::Matrix DOG_mean;
    ivi::Matrix DOG_var;
    extract_texture
    (
        img,
        im,
        Oe_mean,
        Oe_var,
        DOG_mean,
        DOG_var,
        4, //nGaussScales
        12, //nGaussOrientations
        1.41, //GaussSigmaY
        3.0, //GaussX2YSigmaRatio
        4, //nDOGScales
        1.41, //DOGExcitSigma
        0.62, //DOGInhibSigmaRatio1
        1.6, //DOGInhibSigmaRatio2
        0 //nCroppedPixels
    );
    Oe_mean.write(out_oem_name.c_str());
    Oe_var.write(out_oev_name.c_str());
    DOG_mean.write(out_dm_name.c_str());
    DOG_var.write(out_dv_name.c_str());

    ivi::Matrix in_Oe_mean(in_oem_name.c_str());
    ivi::Matrix in_Oe_var(in_oev_name.c_str());
    ivi::Matrix in_DOG_mean(in_dm_name.c_str());
    ivi::Matrix in_DOG_var(in_dv_name.c_str());

    double d1 = ivi::max_abs_difference(Oe_mean, in_Oe_mean);
    double d2 = ivi::max_abs_difference(Oe_var, in_Oe_var);
    double d3 = ivi::max_abs_difference(DOG_mean, in_DOG_mean);
    double d4 = ivi::max_abs_difference(DOG_var, in_DOG_var);
    std::cout << argv[1] << ": " << d1 << " " << d2 << " " << d3 << " " << d4 << std::endl;

    return 0;
}

