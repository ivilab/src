/* $Id: xconvo.cpp 25499 2020-06-14 13:26:04Z kobus $ */

#include <m_cpp/m_convolve.h>
#include <iostream>

int main(int, char**)
{
    const double z[] = { 1,1,1,1,1,1,1,1,
                         1,1,1,0,0,0,1,1,
                         2,2,2,2,2,2,2,2,
                         2,2,2,0,0,0,2,2,
                         0,0,0,0,0,0,0,0 },
                 r[] = { 6,6,6,6,6,6,6,6,
                         6,6,4,2,0,2,4,6,
                         12,12,12,12,12,12,12,12,
                         12,12,8,4,0,4,8,12,
                         0,0,0,0,0,0,0,0 };

    const ivi::Matrix u(5,8, z), rr(5,8, r);
    const ivi::Vector g2(3, 2.0);
    ivi::Matrix s;

    ivi::x_convolve_matrix(&s, u, g2);
	const bool success( s==rr );

    if (ivi_c::is_interactive())
    {
        std::cout << u << '\n'
                  << g2 << "\n\n"
                  << s << '\n';
		if (success)
			std::cout << "success!\n";
		else
			std::cout << "failure!\nLast matrix expected to be\n"
						<< rr << '\n';
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

