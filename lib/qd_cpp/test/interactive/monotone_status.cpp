/*
 * $Id: monotone_status.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <qd_cpp/triangulate.h>

#include <iostream>
#include <fstream>

int fail(const std::string& msg)
{
    std::cerr << "Error: " << msg << '\n';
    return EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    if (argc < 2) return fail("need a filename argument");

    std::ifstream f(argv[1]);
    if (!f) return fail("cannot read from file");

    ivi::qd::Doubly_connected_edge_list d;
    try
    {
        ivi::qd::Doubly_connected_edge_list
            e = ivi::qd::Doubly_connected_edge_list::ctor_xml_stream(f);
        d.swap(e);
    }
    catch (const ivi::Exception& e)
    {
        e.print_details(std::cerr);
        return fail("file parsing error");
    }

    for (size_t i = 0; i < d.get_face_table().size(); ++i)
    {
        std::cout << i << '\t'
            << ivi::qd::is_face_ymonotone(d, i) << '\n';
    }

    return EXIT_SUCCESS;
}
