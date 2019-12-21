#pragma once

#include "tcc/core/core.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace tcc
{
struct BinaryFormat
{
    static auto Write(std::string const& path, std::vector<Integer> const& data) -> bool;
    static auto Read(std::string const& path, std::vector<Integer>& data) -> bool;
};
}  // namespace tcc
