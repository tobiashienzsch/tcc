#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace tcc
{
struct BinaryProgram
{
    int64_t version {0};
    char name[25] {0};
    int64_t entryPoint = {0};
    std::vector<int64_t> data;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& version;
        ar& name;
        ar& entryPoint;
        ar& data;
    }
};

struct BinaryFormat
{
    template<typename StreamType>
    static auto WriteToStream(StreamType& stream, BinaryProgram const& program) -> bool
    {
        auto oa = boost::archive::binary_oarchive(stream);
        oa << program;

        return true;
    }

    template<typename StreamType>
    static auto ReadFromStream(StreamType& stream, BinaryProgram& program) -> bool
    {
        auto inputArchive = boost::archive::binary_iarchive(stream);
        inputArchive >> program;
        return true;
    }

    static auto WriteToFile(std::string const& path, BinaryProgram const& program) -> bool;
    static auto ReadFromFile(std::string const& path, BinaryProgram& program) -> bool;
};

}  // namespace tcc
