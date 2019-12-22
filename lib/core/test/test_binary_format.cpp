/**
 * @file test_binary_format.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/core/binary_format.hpp"

#include <sstream>

TEST_CASE("core: BinaryFormat WriteRead", "[core]")
{
    auto stream = std::stringstream{};

    // write to stream
    {
        auto const program = tcc::BinaryProgram{1, "test", 0, {}};
        tcc::BinaryFormat::WriteToStream(stream, program);
    }

    // read to stream
    {
        auto program = tcc::BinaryProgram{};
        tcc::BinaryFormat::ReadFromStream(stream, program);

        REQUIRE(program.version == 1);
        REQUIRE(program.name == std::string("test"));
        REQUIRE(program.entryPoint == 0);
        REQUIRE(program.data == std::vector<tcc::Integer>{});
    }
}