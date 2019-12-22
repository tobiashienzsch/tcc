#include "tcc/core/binary_format.hpp"

namespace tcc
{

auto BinaryFormat::Write(std::string const& path, std::vector<Integer> const& data) -> bool
{
    // open
    auto file = std::ofstream(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        fmt::print("Could not open file: {}\n", path);
        return false;
    }

    auto program = BinaryProgram{1, "test", data};

    // write archive
    boost::archive::text_oarchive oa(file);
    oa << program;

    // close
    file.close();
    if (!file.good())
    {
        fmt::print("Error occurred at writing time!");
        return false;
    }

    return true;
}

auto BinaryFormat::Read(std::string const& path, std::vector<Integer>& data) -> bool
{
    auto file = std::ifstream(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        fmt::print("Could not open file: {}\n", path);
        return false;
    }

    // open the archive
    auto program = BinaryProgram{};
    boost::archive::text_iarchive ia(file);
    ia >> program;
    data = program.data;

    fmt::print("Name: {}\n", program.name);
    for (auto x : program.data) fmt::print("{}\n", x);

    return true;
}

}  // namespace tcc