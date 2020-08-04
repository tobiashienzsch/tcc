/**
 * @file optimizer_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/optimizer/optimizer.hpp"

#include "catch2/catch.hpp"
#include "tcsl/tcsl.hpp"

using std::string;

using tcc::IRByteCode;
using tcc::IRStatement;
using tcc::Optimizer;

TEST_CASE("tcc/optimizer: BinaryOperation", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRByteCode, bool>({
        {IRByteCode::Call, false},           //
        {IRByteCode::Store, false},          //
        {IRByteCode::Jump, false},           //
        {IRByteCode::Addition, true},        //
        {IRByteCode::Subtraction, true},     //
        {IRByteCode::Multiplication, true},  //
        {IRByteCode::Division, true},        //
    }));

    REQUIRE(Optimizer::isBinaryOperation(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgument", "[tcc][optimizer]")
{
    using Argument = IRStatement::Argument;

    auto [test_input, expected] = GENERATE(table<Argument, bool>({
        {Argument {string("x1")}, false},       //
        {Argument {string("t1")}, false},       //
        {Argument {uint32_t {1}}, true},        //
        {Argument {uint32_t {42}}, true},       //
        {Argument {uint32_t {143}}, true},      //
        {Argument {uint32_t {1111111}}, true},  //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgumentOptional", "[tcc][optimizer]")
{
    using Argument = IRStatement::OptionalArgument;

    auto [test_input, expected] = GENERATE(table<Argument, bool>({
        {Argument {string("x1")}, false},       //
        {Argument {string("t1")}, false},       //
        {Argument {std::nullopt}, false},       //
        {Argument {uint32_t {1}}, true},        //
        {Argument {uint32_t {42}}, true},       //
        {Argument {uint32_t {143}}, true},      //
        {Argument {uint32_t {1111111}}, true},  //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantStoreExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRStatement, bool>({
        {IRStatement {IRByteCode::Store, string("x1"), uint32_t {1}, std::nullopt}, true},  //
        {IRStatement {IRByteCode::Multiplication, string("t78"), uint32_t {123}, uint32_t {1}},
         false},  // not store
        {IRStatement {IRByteCode::Store, string("t1"), string("t0"), std::nullopt},
         false},  // not const
    }));

    REQUIRE(Optimizer::isConstantStoreExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantBinaryExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRStatement, bool>({
        {IRStatement {IRByteCode::Store, string("t2"), string("t0"), std::nullopt},
         false},  // not binary
        {IRStatement {IRByteCode::Multiplication, string("t2"), uint32_t {1}, uint32_t {451}},
         true},  //
    }));

    REQUIRE(Optimizer::isConstantBinaryExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: UnusedStatement", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {IRByteCode::Store, string("x1"), uint32_t {1}, std::nullopt},  //
        IRStatement {IRByteCode::Store, string("x2"), string("x1"), std::nullopt}   //
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("tcc/optimizer: DeleteUnusedStatements", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {IRByteCode::Store, string("t0"), uint32_t {1}, std::nullopt}  //
    };

    REQUIRE(testData.size() == std::size_t {1});
    REQUIRE(Optimizer::DeleteUnusedStatements(testData) == false);
    REQUIRE(testData.size() == std::size_t {0});
}

TEST_CASE("tcc/optimizer: ReplaceVariableIfConstant", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {IRByteCode::Store, string("x1"), uint32_t {143}, std::nullopt},
        IRStatement {IRByteCode::Store, string("x2"), string("x1"), std::nullopt},
        IRStatement {IRByteCode::Store, string("t0"), string("x1"), std::nullopt},
        IRStatement {IRByteCode::Addition, string("t0"), uint32_t {42}, string("x1")}};

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<std::uint32_t>(testData[1].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[2].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[3].second.value()) == 143);
}