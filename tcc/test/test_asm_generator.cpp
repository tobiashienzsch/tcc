/**
 * @file test_asm_generator.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/core/testing.hpp"

#include "asm/generator.hpp"

using tcc::byte_code;
using tcc::ThreeAddressCode;
using asmGen = tcc::AssemblyGenerator;
using std::string;

TEST_CASE("optimizer: BinaryOperation", "[optimizer]")
{
    auto testData = std::vector<tcc::TestCase<byte_code, bool>> {
        {byte_code::op_call, false},   //
        {byte_code::op_store, false},  //
        {byte_code::op_jump, false},   //
        {byte_code::op_add, true},     //
        {byte_code::op_sub, true},     //
        {byte_code::op_mul, true},     //
        {byte_code::op_div, true},     //
    };

    for (auto const& test : testData)
    {
        REQUIRE(asmGen::isBinaryOperation(test.input) == test.expected);
    }
}

TEST_CASE("optimizer: ConstantArgument", "[optimizer]")
{
    using Argument = ThreeAddressCode::Argument;

    auto testData = std::vector<tcc::TestCase<Argument, bool>> {
        {Argument {string("x1")}, false},  //
        {Argument {string("t1")}, false},  //
        {Argument {1}, true},              //
        {Argument {42}, true},             //
        {Argument {143}, true},            //
        {Argument {1111111}, true},        //
    };

    for (auto const& test : testData)
    {
        REQUIRE(asmGen::isConstantArgument(test.input) == test.expected);
    }
}

TEST_CASE("optimizer: ConstantArgumentOptional", "[optimizer]")
{
    using Argument = ThreeAddressCode::OptionalArgument;

    auto testData = std::vector<tcc::TestCase<Argument, bool>> {
        {Argument {string("x1")}, false},  //
        {Argument {string("t1")}, false},  //
        {Argument {std::nullopt}, false},  //
        {Argument {1}, true},              //
        {Argument {42}, true},             //
        {Argument {143}, true},            //
        {Argument {1111111}, true},        //
    };

    for (auto const& test : testData)
    {
        REQUIRE(asmGen::isConstantArgument(test.input) == test.expected);
    }
}

TEST_CASE("optimizer: ConstantStoreExpression", "[optimizer]")
{
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

TEST_CASE("optimizer: ConstantBinaryExpression", "[optimizer]")
{
    auto testData = std::vector<tcc::TestCase<ThreeAddressCode, bool>> {
        {ThreeAddressCode {byte_code::op_store, string("t2"), string("t0"), std::nullopt}, false},  // not binary
        {ThreeAddressCode {byte_code::op_mul, string("t2"), 1, 451}, true},                         //
    };

    for (auto const& test : testData)
    {
        REQUIRE(asmGen::isConstantBinaryExpression(test.input) == test.expected);
    }
}

TEST_CASE("optimizer: UnusedStatement", "[optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {byte_code::op_store, string("x1"), 1, std::nullopt},            //
        ThreeAddressCode {byte_code::op_store, string("x2"), string("x1"), std::nullopt}  //
    };

    REQUIRE(asmGen::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(asmGen::IsUnusedStatement(testData.at(1), testData) == true);
}