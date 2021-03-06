/**
 * @file binary_format_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include <sstream>

#include "catch2/catch.hpp"
#include "tcsl/tcsl.hpp"

TEST_CASE("tcsl: BinaryFormat WriteReadStream", "[tcsl]")
{
    auto stream       = std::stringstream {};
    auto const output = tcc::BinaryProgram {1, "test", 0, {}};
    tcc::BinaryFormat::writeToStream(stream, output);

    SECTION("read to stream")
    {
        auto input = tcc::BinaryProgram {};
        tcc::BinaryFormat::readFromStream(stream, input);

        REQUIRE(input.version == 1);
        REQUIRE(input.name == std::string("test"));
        REQUIRE(input.entryPoint == 0);
        REQUIRE(input.data.empty());
    }
}

TEST_CASE("tcsl: BinaryFormatWriteReadFile", "[tcsl]")
{
    // temporary file
    auto const tempFile = std::string("test_BinaryFormatWriteReadFile.tcb");

    // write to stream
    {
        auto const program = tcc::BinaryProgram {1, "test", 0, {1, 2, 3}};
        tcc::BinaryFormat::writeToFile(tempFile, program);
    }

    // read to stream
    {
        auto program = tcc::BinaryProgram {};
        tcc::BinaryFormat::readFromFile(tempFile, program);

        REQUIRE(program.version == 1);
        REQUIRE(program.name == std::string("test"));
        REQUIRE(program.entryPoint == 0);
        REQUIRE(program.data == std::vector<int64_t> {1, 2, 3});
    }
}