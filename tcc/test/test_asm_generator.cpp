/**
 * @file test_asm_generator.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/core/testing.hpp"

#include "asm/generator.hpp"

using tcc::byte_code;
using tcc::ThreeAddressCode;
auto generateThreeAddressCode() -> std::vector<ThreeAddressCode>
{
    return {
        //
    };
}

TEST_CASE("optimizer: ConstantStoreExpression", "[optimizer]")
{
    using asmGen = tcc::AssemblyGenerator;
    using std::string;
    auto testData = std::vector<tcc::TestCase<ThreeAddressCode, bool>> {
        {ThreeAddressCode {byte_code::op_store, string("x1"), 1, std::nullopt}, true},              //
        {ThreeAddressCode {byte_code::op_mul, string("t78"), 123, 1}, false},                       // not store
        {ThreeAddressCode {byte_code::op_store, string("t1"), string("t0"), std::nullopt}, false},  // not const
    };

    for (auto const& test : testData)
    {
        REQUIRE(asmGen::isConstantStoreExpression(test.input) == test.expected);
    }
}