#include <l_cpp/l_exception.h>

#include <vector>
#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "dbn_cpp/data.h"
#include "dbn_cpp/util.h"
#include "dbn_cpp/linear_state_space.h"

using namespace ivi;
using namespace ivi::ties;
using namespace std;

int main(int argc, const char** argv)
{
    if(argc != 5)
    {
        std::cout << " Usuage: " << argv[0] 
                  << " data-file topics-dir exp-dir out-file\n";
        return EXIT_SUCCESS;
    }

    try{

        string data_dir = argv[1];
        string topics_dir = argv[2];
        string exp_dir = argv[3];
        string out_file = argv[4];
        boost::format data_fmt(data_dir + "/%04d.txt");
        boost::format topics_fmt(topics_dir + "/%04d.txt");
        std::vector<Data> data;
        const size_t min_id = 2; 
        const size_t max_id = 46;
        for(size_t i = min_id; i <= max_id ; i++)
        {
            string fname = (data_fmt % i).str();
            if(ivi_c::is_file(fname.c_str()))
            {
                data.push_back(parse_data(fname));
            }
        }
        
        ofstream out_ofs(out_file.c_str());
        if(out_ofs.fail())
        {
            IVI_THROW_3(IO_error, "Can't open file %s", (out_file.c_str()));
        }

        for(size_t d = 0; d < data.size(); d++)
        {
            //double bmidiff = data[d].bmidiff;
            //double bmiave = data[d].bmiave;
            size_t dyid = data[d].dyid;
            string data_fp = (data_fmt % dyid).str();
            Vector bmi_data(data_fp);

            // Read in the topics file 
            string topics_fname = (topics_fmt % dyid).str();
            vector<pair<size_t, size_t> > topics_times
                = parse_topics(topics_fname);

            for(size_t t = 0; t < topics_times.size(); t++)
            {
                size_t start = topics_times[t].first;
                const Double_v& cur_times = data[d].times;
                int end = std::min((int)topics_times[t].second, (int)cur_times.size() - 1);
                boost::format param_fp_fmt(exp_dir + "/%04d/%02d-%02d/com_params.txt");
                string param_fp = (param_fp_fmt % dyid % start % end).str();

                if(!ivi_c::is_file(param_fp.c_str())) continue;

                Linear_state_space lss;
                lss.read(param_fp);
                const Coupled_oscillator& clo = lss.coupled_oscillators()[0];

                out_ofs << data[d].dyid << " ";
                for(size_t j = 0; j < clo.num_params(); j++)
                {
                    out_ofs << clo.get_param(j) << " ";
                }
                //out_ofs << bmidiff << " " << bmiave <<  std::endl;
                out_ofs << bmi_data[1] << " " << bmi_data[2] <<  std::endl;

            }
        }
    }
    catch(IO_error& e)
    {
        std::cerr << e.get_msg() << endl;
        exit(1);
    }
    catch(Exception& e)
    {
        std::cerr << "Uncaught exception: " << e.get_msg() << endl;
        exit(1);
    }
    catch(std::exception& e)
    {
        std::cerr << "Uncaught exception: " << e.what() << endl;
        exit(1);
    }
    catch(...)
    {
        cerr << "Unknown error occurred" << endl;
        exit(1);
    }
    return EXIT_SUCCESS;
}

