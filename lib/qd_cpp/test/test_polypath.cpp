/**
 * @file
 * @brief unit test for class PolyPath
 * @author Andrew Predoehl
 */
/*
 * $Id: test_polypath.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_init.h>
#include <l_cpp/l_test.h>
#include <qd_cpp/polypath.h>

#include <iostream>

namespace {

struct Ref { bool vertex; int x, y; double dx, dy, ex, ey; };
Ref ref[] =
{
    {  true,    0, 0,    1, 0,                  1, 0    },
    {  false,   1, 0,    1, 0,                  1, 0    },
    {  false,   2, 0,    1, 0,                  1, 0    },
    {  false,   3, 0,    1, 0,                  1, 0    },
    {  false,   4, 0,    1, 0,                  1, 0    },
    {  false,   5, 0,    1, 0,                  1, 0    },
    {  false,   6, 0,    1, 0,                  1, 0    },
    {  false,   7, 0,    1, 0,                  1, 0    },
    {  false,   8, 0,    1, 0,                  1, 0    },
    {  false,   9, 0,    1, 0,                  1, 0    },
    {  true,    10, 0,   0.707107, 0.707107,    0, 1    },
    {  false,   10, 1,   0, 1,                  -1, 0   },
    {  false,   10, 2,   0, 1,                  -1, 0   },
    {  false,   10, 3,   0, 1,                  -1, 0   },
    {  false,   10, 4,   0, 1,                  -1, 0   },
    {  false,   10, 5,   0, 1,                  -1, 0   },
    {  false,   10, 6,   0, 1,                  -1, 0   },
    {  false,   10, 7,   0, 1,                  -1, 0   },
    {  false,   10, 8,   0, 1,                  -1, 0   },
    {  false,   10, 9,   0, 1,                  -1, 0   },
    {  true,    10, 10,  -0.707107, 0.707107,   0, -1   },
    {  false,   9, 10,   -1, 0,                 1, 0    },
    {  false,   8, 10,   -1, 0,                 1, 0    },
    {  false,   7, 10,   -1, 0,                 1, 0    },
    {  false,   6, 10,   -1, 0,                 1, 0    },
    {  false,   5, 10,   -1, 0,                 1, 0    },
    {  false,   4, 10,   -1, 0,                 1, 0    },
    {  false,   3, 10,   -1, 0,                 1, 0    },
    {  false,   2, 10,   -1, 0,                 1, 0    },
    {  false,   1, 10,   -1, 0,                 1, 0    },
    {  true,    0, 10,   -0.707107, 0.707107,   0, -1   },
    {  true,    0, 11,   0.707107, 0.707107,    0, 1    },
    {  true,    1, 11,   0.707107, 0.707107,    0, 1    },
    {  true,    1, 12,   -0.707107, 0.707107,   0, -1   },
    {  true,    0, 12,   -1, 0,                 1, 0    },
};

inline ivi::Vector2 doubler(const ivi::Vector2& v)
{
    return ivi::qd::get_unit_vector_2x_angle_of_unit_vector(v);
}



bool test1()
{
    ivi::qd::PixPath c(ivi::qd::PixPath::reserve(4));

    c.push_back(ivi::qd::PixPoint(0, 0));
    c.push_back(ivi::qd::PixPoint(10, 0));
    c.push_back(ivi::qd::PixPoint(10, 10));
    c.push_back(ivi::qd::PixPoint(0, 10));

    // small sharp kink
    c.push_back(ivi::qd::PixPoint(0, 11));
    c.push_back(ivi::qd::PixPoint(1, 11));
    c.push_back(ivi::qd::PixPoint(1, 12));
    c.push_back(ivi::qd::PixPoint(0, 12));

    ivi::qd::PolyPath pp(ivi::qd::PolyPath::construct_from_vertices(c));

    if (ivi_c::is_interactive())
    {
        pp.debug_print(std::cout);
    }

    const size_t N = sizeof ref / sizeof(Ref);
    TEST_TRUE(N == pp.size());

    for (size_t iii = 0; iii < N; ++iii)
    {
        TEST_TRUE(pp.is_a_vertex(iii) == ref[iii].vertex);
        TEST_TRUE(pp[iii].x == ref[iii].x);
        TEST_TRUE(pp[iii].y == ref[iii].y);
        // precision here is bottlenecked by the above reference accuracy
        TEST_TRUE(fabs(pp.tangent_at(iii).x() - ref[iii].dx) < 5e-7);
        TEST_TRUE(fabs(pp.tangent_at(iii).y() - ref[iii].dy) < 5e-7);

        // try out the angle-doubling function also
        const ivi::Vector2 dubang = doubler(pp.tangent_at(iii));
        TEST_TRUE(fabs(dubang.x() - ref[iii].ex) < 5e-7);
        TEST_TRUE(fabs(dubang.y() - ref[iii].ey) < 5e-7);
    }

    return true;
}

// test for is_valid_as_polypath()
bool test2()
{
    ivi::qd::PixPath p(ivi::qd::PixPath::reserve(5));
    ivi::qd::PixPath q(ivi::qd::PixPath::reserve(5));

    p.push_back(ivi::qd::PixPoint(32, 31));
    p.push_back(ivi::qd::PixPoint(35, 35));
    p.push_back(ivi::qd::PixPoint(38, 39)); // index 2, the hinge of a whisker
    p.push_back(ivi::qd::PixPoint(29, 27));
    p.push_back(ivi::qd::PixPoint(30, 30));

    TEST_FALSE(is_valid_as_polypath(p));

    bool caught = false;
    try
    {
        is_valid_as_polypath(p, true);
    }
    catch (ivi::Exception& e)
    {
        caught = true;
        size_t k=e.get_msg().find("Input has a whisker bend around index 2 ");
        TEST_TRUE(k < e.get_msg().size());
    }
    TEST_TRUE(caught);

    q.push_back(ivi::qd::PixPoint(35, 35));
    q.push_back(ivi::qd::PixPoint(38, 39));
    q.push_back(ivi::qd::PixPoint(27, 29));
    q.push_back(ivi::qd::PixPoint(30, 30));

    TEST_TRUE(is_valid_as_polypath(q));

    q.push_back(ivi::qd::PixPoint(38, 39));

    TEST_FALSE(is_valid_as_polypath(q));

    caught = false;
    try
    {
        is_valid_as_polypath(q, true);
    }
    catch (ivi::Exception& e)
    {
        caught = true;
        size_t k = e.get_msg().find("Input path has a duplicate point "
                                    "at (38, 39) which is hit 2 times");
        TEST_TRUE(k < e.get_msg().size());
    }
    TEST_TRUE(caught);

    return true;
}

}


int main(int argc, char *argv[])
{
    IVI(EPETE(ivi_init()));
    TEST_TRUE(test1());
    TEST_TRUE(test2());
    ivi_c::ivi_cleanup();
    RETURN_VICTORIOUSLY();
}

