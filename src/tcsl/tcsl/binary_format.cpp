/**
 * @file binary_format.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcsl/binary_format.hpp"

#include "fmt/format.h"

namespace tcc
{

auto BinaryFormat::writeToFile(std::string const& path, BinaryProgram const& program) -> bool
{
    // open
    auto file = std::ofstream(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        fmt::print("Could not open file: {}\n", path);
        return false;
    }

    writeToStream(file, program);

    // close
    file.close();
    if (!file.good())
    {
        fmt::print("Error occurred at writing time!");
        return false;
    }

    return true;
}

auto BinaryFormat::readFromFile(std::string const& path, BinaryProgram& program) -> bool
{
    auto file = std::ifstream(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        fmt::print("Could not open file: {}\n", path);
        return false;
    }

    return readFromStream(file, program);
}

}  // namespace tcc