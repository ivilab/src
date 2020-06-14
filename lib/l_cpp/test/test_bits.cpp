/**
 * @file
 * @brief unit test for lib/l_cpp/l_bits.h (which does endian-flips)
 * @author andrew predoehl
 */
/*
 * $Id: test_bits.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include "l/l_init.h"
#include "l/l_sys_io.h"
#include "l/l_sys_lib.h"
#include "l/l_sys_debug.h"
#include "l_cpp/l_cpp_bits.h"
#include "l_cpp/l_util.h"

#include <algorithm>

#define FRF(x) do { if (EXIT_FAILURE==(x)) return EXIT_FAILURE; } while(0)

namespace {

const char *s = "fistular", *t = "ralutsif";
size_t SMAX = strlen(s);


template <typename U>
int test_swapping(U& ux)
{
    const size_t usize = sizeof(ux.a);
    const char *answer = t + SMAX - usize;

    std::copy(s, s+usize, ux.a);
    //std::string qf(ux.a, usize);
    ivi::swap_bytes(ux.x);
    //std::string qr(ux.a, usize);

    //std::cout << "Input: '" << qf << "'; output: '" << qr << "' -- wow.\n";
    if (0 == ivi_c::ivi_strncmp(ux.a, answer, usize)) return EXIT_SUCCESS;

    ivi_c::pso("Error for input of size %u\n", usize);
    return EXIT_FAILURE;
}


union U2 { char a[2]; ivi_c::ivi_uint16 x; } u2;
union U4 { char a[4]; ivi_c::ivi_uint32 x; } u4;
union U8 { char a[8]; ivi_c::ivi_uint64 x; } u8;

}

int main()
{
    ivi_c::ivi_init();

    ASSERT(2 == sizeof(U2));
    ASSERT(4 == sizeof(U4));
    ASSERT(8 == sizeof(U8));

    FRF(test_swapping(u2));
    FRF(test_swapping(u4));
    FRF(test_swapping(u8));

    ivi_c::ivi_cleanup();

    return EXIT_SUCCESS;
}

