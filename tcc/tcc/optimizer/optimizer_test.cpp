/**
 * @file optimizer_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/optimizer/optimizer.hpp"

#include "catch2/catch.hpp"
#include "tsl/tsl.hpp"

using std::string;

using tcc::IRByteCode;
using tcc::Optimizer;
using tcc::ThreeAddressCode;

TEST_CASE("tcc/optimizer: BinaryOperation", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRByteCode, bool>({
        {IRByteCode::op_call, false},   //
        {IRByteCode::op_store, false},  //
        {IRByteCode::op_jump, false},   //
        {IRByteCode::op_add, true},     //
        {IRByteCode::op_sub, true},     //
        {IRByteCode::op_mul, true},     //
        {IRByteCode::op_div, true},     //
    }));

    REQUIRE(Optimizer::isBinaryOperation(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgument", "[tcc][optimizer]")
{
    using Argument = ThreeAddressCode::Argument;

    auto [test_input, expected] = GENERATE(table<Argument, bool>({
        {Argument {string("x1")}, false},  //
        {Argument {string("t1")}, false},  //
        {Argument {1}, true},              //
        {Argument {42}, true},             //
        {Argument {143}, true},            //
        {Argument {1111111}, true},        //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgumentOptional", "[tcc][optimizer]")
{
    using Argument = ThreeAddressCode::OptionalArgument;

    auto [test_input, expected] = GENERATE(table<Argument, bool>({
        {Argument {string("x1")}, false},  //
        {Argument {string("t1")}, false},  //
        {Argument {std::nullopt}, false},  //
        {Argument {1}, true},              //
        {Argument {42}, true},             //
        {Argument {143}, true},            //
        {Argument {1111111}, true},        //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantStoreExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<ThreeAddressCode, bool>({
        {ThreeAddressCode {IRByteCode::op_store, string("x1"), 1, std::nullopt}, true},              //
        {ThreeAddressCode {IRByteCode::op_mul, string("t78"), 123, 1}, false},                       // not store
        {ThreeAddressCode {IRByteCode::op_store, string("t1"), string("t0"), std::nullopt}, false},  // not const
    }));

    REQUIRE(Optimizer::isConstantStoreExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantBinaryExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<ThreeAddressCode, bool>({
        {ThreeAddressCode {IRByteCode::op_store, string("t2"), string("t0"), std::nullopt}, false},  // not binary
        {ThreeAddressCode {IRByteCode::op_mul, string("t2"), 1, 451}, true},                         //
    }));

    REQUIRE(Optimizer::isConstantBinaryExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: UnusedStatement", "[tcc][optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {IRByteCode::op_store, string("x1"), 1, std::nullopt},            //
        ThreeAddressCode {IRByteCode::op_store, string("x2"), string("x1"), std::nullopt}  //
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("tcc/optimizer: DeleteUnusedStatements", "[tcc][optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {IRByteCode::op_store, string("t0"), 1, std::nullopt}  //
    };

    REQUIRE(testData.size() == std::size_t {1});
    REQUIRE(Optimizer::DeleteUnusedStatements(testData) == false);
    REQUIRE(testData.size() == std::size_t {0});
}

TEST_CASE("tcc/optimizer: ReplaceVariableIfConstant", "[tcc][optimizer]")
{
    auto testData = tcc::StatementList {
        ThreeAddressCode {IRByteCode::op_store, string("x1"), 143, std::nullopt},           //
        ThreeAddressCode {IRByteCode::op_store, string("x2"), string("x1"), std::nullopt},  //
        ThreeAddressCode {IRByteCode::op_store, string("t0"), string("x1"), std::nullopt},  //
        ThreeAddressCode {IRByteCode::op_add, string("t0"), 42, string("x1")}               //
    };

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<int>(testData[1].first) == 143);
    REQUIRE(std::get<int>(testData[2].first) == 143);
    REQUIRE(std::get<int>(testData[3].second.value()) == 143);
}