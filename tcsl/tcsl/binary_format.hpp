/**
 * @file binary_format.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace tcc
{
struct BinaryProgram
{
    int64_t version {0};
    std::string name {""};
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
        boost::archive::binary_oarchive oa(stream);
        oa << program;

        return true;
    }

    template<typename StreamType>
    static auto ReadFromStream(StreamType& stream, BinaryProgram& program) -> bool
    {
        boost::archive::binary_iarchive inputArchive(stream);
        inputArchive >> program;
        return true;
    }

    static auto WriteToFile(std::string const& path, BinaryProgram const& program) -> bool;
    static auto ReadFromFile(std::string const& path, BinaryProgram& program) -> bool;
};

}  // namespace tcc
