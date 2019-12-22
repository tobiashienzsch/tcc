#pragma once

#include "tcc/core/core.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace tcc
{
struct TcbHeader
{
    int64_t version    = {0};
    char name[25]      = {0};
    int64_t dataSize   = {0};
    int64_t entryPoint = {0};
};

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

struct BinaryFormat
{
    static auto Write(std::string const& path, std::vector<Integer> const& data) -> bool;
    static auto Read(std::string const& path, std::vector<Integer>& data) -> bool;
};
}  // namespace tcc
