/**
 * @file test_optimizer.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/core.hpp"

#include "optimizer/optimizer.hpp"

using std::string;

using tcc::byte_code;
using tcc::Optimizer;
using tcc::ThreeAddressCode;

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
        REQUIRE(Optimizer::isBinaryOperation(test.input) == test.expected);
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
        REQUIRE(Optimizer::isConstantArgument(test.input) == test.expected);
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
        REQUIRE(Optimizer::isConstantArgument(test.input) == test.expected);
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
        REQUIRE(Optimizer::isConstantStoreExpression(test.input) == test.expected);
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
        REQUIRE(Optimizer::isConstantBinaryExpression(test.input) == test.expected);
    }
}

TEST_CASE("optimizer: UnusedStatement", "[optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {byte_code::op_store, string("x1"), 1, std::nullopt},            //
        ThreeAddressCode {byte_code::op_store, string("x2"), string("x1"), std::nullopt}  //
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("optimizer: DeleteUnusedStatements", "[optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {byte_code::op_store, string("t0"), 1, std::nullopt}  //
    };

    REQUIRE(testData.size() == std::size_t {1});
    REQUIRE(Optimizer::DeleteUnusedStatements(testData) == false);
    REQUIRE(testData.size() == std::size_t {0});
}

TEST_CASE("optimizer: ReplaceVariableIfConstant", "[optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {byte_code::op_store, string("x1"), 143, std::nullopt},           //
        ThreeAddressCode {byte_code::op_store, string("x2"), string("x1"), std::nullopt},  //
        ThreeAddressCode {byte_code::op_store, string("t0"), string("x1"), std::nullopt},  //
        ThreeAddressCode {byte_code::op_add, string("t0"), 42, string("x1")}               //
    };

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<int>(testData[1].first) == 143);
    REQUIRE(std::get<int>(testData[2].first) == 143);
    REQUIRE(std::get<int>(testData[3].second.value()) == 143);
}