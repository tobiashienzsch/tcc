/**
 * @file optimizer_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/optimizer/optimizer.hpp"

#include "catch2/catch.hpp"
#include "tcsl/tcsl.hpp"

using namespace std::string_literals;

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
        {Argument {"x1"s}, false},              //
        {Argument {"t1"s}, false},              //
        {Argument {1U}, true},                  //
        {Argument {42u}, true},                 //
        {Argument {143u}, true},                //
        {Argument {uint32_t {1111111}}, true},  //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgumentOptional", "[tcc][optimizer]")
{
    using Argument = IRStatement::OptionalArgument;

    auto [test_input, expected] = GENERATE(table<Argument, bool>({
        {Argument {"x1"s}, false},              //
        {Argument {"t1"s}, false},              //
        {Argument {std::nullopt}, false},       //
        {Argument {1U}, true},                  //
        {Argument {42u}, true},                 //
        {Argument {143u}, true},                //
        {Argument {uint32_t {1111111}}, true},  //
    }));

    REQUIRE(Optimizer::isConstantArgument(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantStoreExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRStatement, bool>({
        {IRStatement {IRByteCode::Store, "x1"s, 1U, std::nullopt}, true},      //
        {IRStatement {IRByteCode::Multiplication, "t78"s, 123U, 1U}, false},   // not store
        {IRStatement {IRByteCode::Store, "t1"s, "t0"s, std::nullopt}, false},  // not const
    }));

    REQUIRE(Optimizer::isConstantStoreExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: ConstantBinaryExpression", "[tcc][optimizer]")
{
    auto [test_input, expected] = GENERATE(table<IRStatement, bool>({
        {IRStatement {IRByteCode::Store, "t2"s, "t0"s, std::nullopt}, false},  // not binary
        {IRStatement {IRByteCode::Multiplication, "t2"s, 1U, 451U}, true},     //
    }));

    REQUIRE(Optimizer::isConstantBinaryExpression(test_input) == expected);
}

TEST_CASE("tcc/optimizer: UnusedStatement", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {IRByteCode::Store, "x1"s, 1U, std::nullopt},    //
        IRStatement {IRByteCode::Store, "x2"s, "x1"s, std::nullopt}  //
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("tcc/optimizer: DeleteUnusedStatements", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {IRByteCode::Store, "t0"s, 1U, std::nullopt}  //
    };

    REQUIRE(testData.size() == std::size_t {1});
    REQUIRE(Optimizer::DeleteUnusedStatements(testData) == false);
    REQUIRE(testData.size() == std::size_t {0});
}

TEST_CASE("tcc/optimizer: ReplaceVariableIfConstant", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {IRStatement {IRByteCode::Store, "x1"s, 143u, std::nullopt},
                                          IRStatement {IRByteCode::Store, "x2"s, "x1"s, std::nullopt},
                                          IRStatement {IRByteCode::Store, "t0"s, "x1"s, std::nullopt},
                                          IRStatement {IRByteCode::Addition, "t0"s, 42u, "x1"s}};

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<std::uint32_t>(testData[1].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[2].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[3].second.value()) == 143);
}