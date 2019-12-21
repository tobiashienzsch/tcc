#pragma once

#include "tcc/core/core.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace tcc
{
struct TcbHeader
{
    int64_t version    = {0};
    char name[25]      = {0};
    int64_t dataSize   = {0};
    int64_t entryPoint = {0};
};

struct BinaryFormat
{
    static auto Write(std::string const& path, std::vector<Integer> const& data) -> bool;
    static auto Read(std::string const& path, std::vector<Integer>& data) -> bool;
};
}  // namespace tcc
