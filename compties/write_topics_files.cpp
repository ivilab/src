
#include <string> 
#include <vector>
#include <fstream>

#include <boost/format.hpp>

#include "dbn_cpp/data.h"

using namespace kjb;
using namespace kjb::ties;
using namespace std;

int main(int argc, const char** argv)
{
    string topics_dir = argv[1];
    string data_fp = argv[2];
    Data data = parse_data(data_fp);

    size_t end_index = data.times.size() - 1;
    size_t start_index = 0;
    boost::format out_fmt (topics_dir + "/%04d.txt");
    string out_fp = (out_fmt % data.dyid).str();
    ofstream ofs(out_fp.c_str());
    ofs << start_index << ", " << end_index << std::endl;
    ofs.close();

    return EXIT_SUCCESS;

}

