/**
 * @file asm_utils_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/asm/asm_utils.hpp"

#include "tcsl/tcsl.hpp"

#include "catch2/catch.hpp"

#include <iostream>

using namespace std::string_literals;
using namespace Catch::Matchers;

TEST_CASE("tcc/assembly: ASMUtil::PrettyPrint", "[tcc][assembly]")
{
    SECTION("1")
    {
        std::ostringstream stream;
        tcc::ASMUtils::PrettyPrint(stream, tcc::Assembly {
                                               std::vector<int64_t> {
                                                   tcc::ByteCode::ICONST, 2,  //
                                                   tcc::ByteCode::HALT,       //
                                               },
                                               0,
                                           });
        auto const str = stream.str();

        CHECK_THAT(str, Contains("ICONST,\t2"));
        CHECK_THAT(str, Contains("HALT"));
    }

    SECTION("2")
    {
        std::ostringstream stream;
        tcc::ASMUtils::PrettyPrint(stream, tcc::Assembly {
                                               std::vector<int64_t> {
                                                   tcc::ByteCode::ICONST, 1,  //
                                                   tcc::ByteCode::ICONST, 2,  //
                                                   tcc::ByteCode::IADD,       //
                                                   tcc::ByteCode::RET,        //
                                               },
                                               0,
                                           });
        auto const str = stream.str();

        CHECK_THAT(str, Contains("ICONST,\t1"));
        CHECK_THAT(str, Contains("ICONST,\t2"));
        CHECK_THAT(str, Contains("IADD"));
        CHECK_THAT(str, Contains("RET"));
    }
}