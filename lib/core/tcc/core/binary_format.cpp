#include "tcc/core/binary_format.hpp"

namespace tcc
{

auto BinaryFormat::Write(std::string const& path, std::vector<Integer> const& data) -> bool
{
    auto file = std::ofstream(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        fmt::print("Could not open file: {}\n", path);
        return false;
    }

    auto test = TcbHeader{1, "test", 0, 0};
    file.write((char*)&test, sizeof(TcbHeader));

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

    auto test = TcbHeader{};
    file.read((char*)&test, sizeof(TcbHeader));

    fmt::print("header:\n");
    fmt::print("\tversion: {}\n", test.version);
    fmt::print("\tname: {}\n", test.name);
    fmt::print("\tdataSize: {}\n", test.dataSize);
    fmt::print("\tentryPoint: {}\n", test.entryPoint);

    file.close();
    if (!file.good())
    {
        fmt::print("Error occurred at reading time!\n");
        return false;
    }

    return true;
}

}  // namespace tcc