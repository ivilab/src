/**
 * @file
 * @brief unit test for monotone regularization
 * @author Andy Predoehl
 *
 * Basic test for efficacy of monotone.cpp functions.
 * If you turn on VISUALIZATION it will draw you a picture.
 */
/*
 * $Id: monotone_1.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_init.h>
#include <l_cpp/l_test.h>
#include <qd_cpp/triangulate.h>
#include <qd_cpp/svg_dcel.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace
{

const bool VISUALIZATION = false;

ivi::qd::Doubly_connected_edge_list test_polygon()
{
    ivi::qd::PixPath p = ivi::qd::PixPath::reserve(25);
    p.push_back(ivi::qd::PixPoint(19,  9));
    p.push_back(ivi::qd::PixPoint(21, 15));
    p.push_back(ivi::qd::PixPoint(16, 13));
    p.push_back(ivi::qd::PixPoint(15, 23));
    p.push_back(ivi::qd::PixPoint(13, 20));
    p.push_back(ivi::qd::PixPoint(12, 20));
    p.push_back(ivi::qd::PixPoint(11, 21));
    p.push_back(ivi::qd::PixPoint(10, 20));
    p.push_back(ivi::qd::PixPoint( 9, 20));
    p.push_back(ivi::qd::PixPoint( 8, 23));
    p.push_back(ivi::qd::PixPoint( 6, 23));
    p.push_back(ivi::qd::PixPoint( 4, 23));
    p.push_back(ivi::qd::PixPoint( 3, 19));
    p.push_back(ivi::qd::PixPoint( 6, 16));
    p.push_back(ivi::qd::PixPoint( 5, 12));
    p.push_back(ivi::qd::PixPoint( 2, 14));
    p.push_back(ivi::qd::PixPoint( 1, 10));
    p.push_back(ivi::qd::PixPoint( 4,  5));
    p.push_back(ivi::qd::PixPoint( 7,  7));
    p.push_back(ivi::qd::PixPoint( 8,  7));
    p.push_back(ivi::qd::PixPoint( 9,  7));
    p.push_back(ivi::qd::PixPoint(10,  1));
    p.push_back(ivi::qd::PixPoint(12,  1));
    p.push_back(ivi::qd::PixPoint(14,  1));
    p.push_back(ivi::qd::PixPoint(12, 11));
    return ivi::qd::Doubly_connected_edge_list::ctor_closed_path(p);
}



int test1()
{
    ivi::qd::Doubly_connected_edge_list d = test_polygon();

    TEST_FALSE(is_face_ymonotone(d, 1));

    if (VISUALIZATION)
    {
        std::string sd = draw_dcel_as_svg(d);
        const size_t ct = sd.find("<!-- Text ");
        IVI(ASSERT( ct < sd.size() ));
#if MONOTONE_CPP_DEBUG
        sd.insert(ct, db_fence_labels(d, 1, 25));
#endif
        std::ofstream ff("monotone_1a.svg");
        ff << sd;
    }

    return ivi_c::NO_ERROR;
}


int test2()
{
    using ivi::qd::Doubly_connected_edge_list;

    const Doubly_connected_edge_list d = test_polygon(),
                               mon = ivi::qd::make_a_face_ymonotone(d, 1);

    for (size_t i = 1; i < mon.get_face_table().size(); ++i)
    {
        if (ivi_c::is_interactive()) std::cout << "trying face " << i << '\n';
        TEST_TRUE(is_face_ymonotone(mon, i));
    }

    const std::string s = xml_output(d);
    std::istringstream t(s);
    TEST_TRUE(d == Doubly_connected_edge_list::ctor_xml_stream(t));

    if (VISUALIZATION)
    {
        std::string sd = draw_dcel_as_svg(mon);
        std::ofstream ff("monotone_1b.svg");
        ff << sd;
    }
    return ivi_c::NO_ERROR;
}


}

int main(int /*argc*/, char** /*argv*/)
{
    IVI(EPETE(ivi_init()));
    IVI(EPETE(test1()));
    IVI(EPETE(test2()));

    ivi_c::ivi_cleanup();
    RETURN_VICTORIOUSLY();
}

