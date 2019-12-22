#include <cstdlib>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <fmt/format.h>

namespace fs = std::filesystem;

struct BinaryProgram
{
    int64_t version{0};
    char name[25]{0};
    std::vector<int64_t> data;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& version;
        ar& name;
        ar& data;
    }
};

int main(int, char**)
{

    std::cout << "Temp directory is " << fs::temp_directory_path() << '\n';
    {
        // make an archive
        auto program = BinaryProgram{1, "test", {0, 1, 2, 3, 1, 4, 5, 2, 6, 4, 8, 2}};
        std::ofstream ofs("test.tcb");
        boost::archive::text_oarchive oa(ofs);
        oa << program;
    }

    {
        // open the archive
        auto program = BinaryProgram{};
        std::ifstream ifs("test.tcb");
        boost::archive::text_iarchive ia(ifs);

        // restore the schedule from the archive
        ia >> program;

        fmt::print("Name: {}\n", program.name);
        for (auto x : program.data) fmt::print("{}\n", x);
    }

    return EXIT_SUCCESS;
}