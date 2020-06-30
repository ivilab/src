/**
 * @file unit test for multithreaded convolution
 * @author Andrew Predoehl
 */
/*
 * $Id: test_mt_fft.cpp 25597 2020-06-30 23:31:45Z kobus $
 */

#include <l_cpp/l_util.h>
#include <l_mt/l_mt_util.h>
#include <l_mt_cpp/l_mt_mutexlock.h>
#include <m2/m2_ncc.h>
#include <m_cpp/m_convolve.h>
#include <iostream>

namespace 
{

const int BURDEN = 20;

const int MASK_SIZE = 100;
const int IMAGE_SIZE = 500;

ivi::Fftw_convolution_2d *pcon = NULL;

ivi_c::ivi_pthread_mutex_t mtx = IVI_PTHREAD_MUTEX_INITIALIZER;

struct Convolution_context
{
    const ivi::Matrix *input;
    ivi::Matrix *output;
};

void* thread_workbench(void *v)
{
    using namespace ivi;
    using namespace ivi_c;

    Convolution_context* cc = static_cast<Convolution_context*>(v);
    NRN(pcon);
    NRN(cc);

    // sequentially get work buffer
    Fftw_convolution_2d::Work_buffer wb;
    if(1) { Mutex_lock l(&mtx); wb = pcon -> allocate_work_buffer(); } 

    // convolve -- the slowest step (except maybe for the print statements)
    pcon -> convolve(*(cc -> input), *(cc -> output), wb);

    // sequentially kill the work buffer by clobbering its contents
    if (!work_buffer_is_unique(wb)) {
        set_error("wb not unique in thread %d", get_ivi_pthread_number());
        return NULL;
    }
    if(1) { Mutex_lock l(&mtx); wb = Fftw_convolution_2d::Work_buffer(); }

    return v;
}


int test_1()
{
    std::vector<ivi::Matrix> queue(BURDEN), blur_1(BURDEN), blur_2(BURDEN);

    const ivi::Matrix mask(ivi::create_random_matrix(MASK_SIZE, MASK_SIZE));

    for (int i = 0; i < BURDEN; ++i)
    {
        ivi::Matrix a(ivi::create_random_matrix(IMAGE_SIZE, IMAGE_SIZE));
        ivi_c::Matrix *bc = NULL;
        IVI(EPETE(fourier_convolve_matrix(&bc,
                                    a.get_c_matrix(), mask.get_c_matrix())));
        ivi::Matrix b(bc);
        queue[i].swap(a);
        blur_1[i].swap(b);
    }

    ivi::Fftw_convolution_2d con(IMAGE_SIZE, IMAGE_SIZE, MASK_SIZE, MASK_SIZE);
    con.set_mask(mask);
    pcon = &con;

    std::vector <ivi_c::ivi_pthread_t> vt(BURDEN);
    std::vector < Convolution_context > vb(BURDEN);

    for (int i = 0; i < BURDEN; ++i)
    {
        vb[i].input = &queue[i];
        vb[i].output = &blur_2[i];
    }

    for (int i = 0; i < BURDEN; ++i)
    {
        IVI(ERE(ivi_pthread_create(&vt[i], NULL, thread_workbench, &vb[i])));
    }

    for (int i = 0; i < BURDEN; ++i)
    {
        void *v;
        IVI(ERE(ivi_pthread_join(vt[i], &v)));
        IVI(NRE(v));
    }
    pcon = 0; // just to be tidy

    for (int i = 0; i < BURDEN; ++i)
    {
        std::cout << 1+i << ". max_abs_diff = "
                  << ivi::max_abs_difference(blur_1[i], blur_2[i]) << '\n';
    }

    return ivi_c::NO_ERROR;
}

}

int main(int argc, char *argv[])
{
    try
    {
        IVI(EPETE(test_1()));
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }

    return EXIT_SUCCESS;
}

