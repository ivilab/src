/**
 * @file
 * @brief unit test for doubly connected edge list
 * @author Andrew Predoehl
 */
/*
 * $Id: test_dcel.cpp 25818 2020-09-21 17:32:46Z kobus $
 */

#include "l/l_sys_std.h"
#include "l/l_sys_rand.h"
#include "l/l_init.h"
#include "qd_cpp/ratpoint.h"
#include "qd_cpp/dcel.h"
#include "qd_cpp/svg_dcel.h"

#include <fstream>

namespace
{

using ivi::qd::Doubly_connected_edge_list;
using ivi::qd::RatPoint_line_segment;
using ivi::qd::RatPoint;



int test1()
{
    const Doubly_connected_edge_list d;
    IVI(ERE(is_valid(d)));
    return ivi_c::NO_ERROR;
}


int test2()
{
    const RatPoint_line_segment s(RatPoint(1, 1), RatPoint(2, 1));
    const Doubly_connected_edge_list d(s);
    IVI(ERE(is_valid(d)));
    return ivi_c::NO_ERROR;
}


int test3()
{
    const RatPoint_line_segment s1(RatPoint(1, 1), RatPoint(2, 1)),
                                s2(RatPoint(3, 0), RatPoint(3, 2));
    const Doubly_connected_edge_list d1(s1);
    const Doubly_connected_edge_list d2(s2);
    const Doubly_connected_edge_list d3(d1.merge(d2));
    IVI(ERE(is_valid(d1)));
    IVI(ERE(is_valid(d2)));
    IVI(ERE(is_valid(d3)));
    return ivi_c::NO_ERROR;
}


int test4()
{
    const RatPoint_line_segment s1(RatPoint(1, 1), RatPoint(2, 1)),
                                s2(RatPoint(1, 0), RatPoint(1, 2));
    const Doubly_connected_edge_list d1(s1);
    const Doubly_connected_edge_list d2(s2);
    d1.merge(d2);
    const Doubly_connected_edge_list d3(d1.merge(d2));
    IVI(ERE(is_valid(d1)));
    IVI(ERE(is_valid(d2)));
    IVI(ERE(is_valid(d3)));
    return ivi_c::NO_ERROR;
}


Doubly_connected_edge_list square()
{
    const RatPoint_line_segment s1(RatPoint(1, -1), RatPoint(1, 1));
    const RatPoint::Rat rot[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
    const Doubly_connected_edge_list d1(s1);
    ETX(is_valid(d1));

    Doubly_connected_edge_list d2(d1), d3(d1), d4;
    d2.transform(rot);
    d3.translate(RatPoint(-2,0));
    d4 = d3;
    d4.transform(rot);
    ETX(is_valid(d2));
    ETX(is_valid(d3));
    ETX(is_valid(d4));

    const Doubly_connected_edge_list d12(d1.merge(d2));
    ETX(is_valid(d12));
    const Doubly_connected_edge_list d34(d3.merge(d4));
    ETX(is_valid(d34));

    return d12.merge(d34);
}


int test5()
{
    const Doubly_connected_edge_list d1234(square());
    IVI(ERE(is_valid(d1234)));
    return ivi_c::NO_ERROR;
}


int test6()
{
    const RatPoint_line_segment s1(RatPoint(3, 0), RatPoint(-3, 0));
    const Doubly_connected_edge_list d1(square()), d4(s1);
    Doubly_connected_edge_list d2(d1);
    const RatPoint::Rat grow[9] = {2, 0, 0, 0, 2, 0, 0, 0, 1};
    d2.transform(grow);
    const Doubly_connected_edge_list d3(d2.merge(d1));
    const Doubly_connected_edge_list d5(d3.merge(d4));
    IVI(ERE(is_valid(d5)));
    
    std::ofstream f("test6.svg");
    // Kobus: 2020/09/21
    // Apparntly, std::ofstream is no longer comparable to NULL. Skip check for
    // now, but FIXME! 
    // IVI(NRE(f));
    f << ivi::qd::draw_dcel_as_svg(d5) << '\n';
    return ivi_c::NO_ERROR;
}

int test7()
{
    ivi::qd::PixPoint p1(0,0), p2(0,0);
    Doubly_connected_edge_list d;
    for (size_t t = 1; t < 100; ++t)
    {
        const ivi::qd::PixPoint p(t, 10*ivi_c::ivi_rand()+100*sin(0.0628*t));
        ivi::qd::PixPoint_line_segment s(p,p);
        if (t&1)
        {
            s.a = p1;
            p1 = p;
        }
        else
        {
            s.a = p2;
            p2 = p;
        }
        Doubly_connected_edge_list e(s), f(d.merge(e));
        d.swap(f);
    }

    std::ofstream f("test7.svg");
    // Kobus: 2020/09/21
    // Apparntly, std::ofstream is no longer comparable to NULL. Skip check for
    // now, but FIXME! 
    // IVI(NRE(f));
    f << ivi::qd::draw_dcel_as_svg(d) << '\n';
    return ivi_c::NO_ERROR;
}


}


int main(int argc, char** argv)
{
    IVI(ivi_init());

    try
    {
        IVI(EPETE(test1()));
        IVI(EPETE(test2()));
        IVI(EPETE(test3()));
        IVI(EPETE(test4()));
        IVI(EPETE(test5()));
        IVI(EPETE(test6()));
        IVI(EPETE(test7()));
    }
    catch (const ivi::Exception& e)
    {
        e.print_details_exit();
    }
    catch (const std::exception& e)
    {
        std::cerr << "std::exception caught: " << e.what() << '\n';
    }

    ivi_c::ivi_cleanup();
    return EXIT_SUCCESS;
}
