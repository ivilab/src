/**
 * @file
 * @brief demo the reduce_pixels_pv function
 * @author Andrew Predoehl
 */
/*
 * $Id: test_reduce.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_sys_rand.h>
#include <qd_cpp/pathresize.h>
#include <qd_cpp/svgwrap.h>

#include <iostream>

namespace {

int main2()
{
    using ivi_c::ivi_rand;
    using ivi::qd::PixPoint;

    int SIZE = 100, MAG = 3;
    ivi::qd::PixPath p(ivi::qd::PixPath::reserve(1000));
    p.push_back(PixPoint(5 + SIZE*ivi_rand(), 5 + SIZE*ivi_rand()));

    for (int i=0; i<30; ++i)
    {
        p.append_no_overlap(
            ivi::qd::bresenham_line(
                p.back(), 
                PixPoint(5 + SIZE*ivi_rand(), 5 + SIZE*ivi_rand())
            ));
    }

    const ivi::qd::PixPath  r(ivi::qd::reduce_pixels_pv(p)),
                            t(p.cull_redundant_points()),
                            u(ivi::qd::reduce_pixels_bfs(p));

    ivi::qd::SvgWrap sp(p), sr(r), st(t), su(u);
    sp.set_color("blue").set_svg(1,0).set_text(false);
    st.set_xml(false).set_svg(false).set_color("green");
    sr.set_xml(false).set_svg(false);
    su.set_xml(false).set_svg(0,1).set_color("orange");

    // only needed by the one generating the opening SVG tag.
    sp.m_width = sp.m_height = 5 + SIZE + 5;
    sp.m_magnify = MAG;

    std::cout << sp() << '\n' << st() << '\n' << sr() << '\n' << st() << '\n';
    std::cerr <<"<-- reduced size: " << r.size() << " -->\n"
                "<-- culled size: " << t.size() << " -->\n"
                "<-- tree-search size: " << u.size() << " -->\n";

    return 0;
}

}

int main()
{
    try {
        return main2();
    }
    catch (ivi::Exception& e) {
        e.print_details_exit();
    }
}
