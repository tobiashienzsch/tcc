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
    auto [testInput, expected] = GENERATE(table<IRByteCode, bool>({
        {IRByteCode::Call, false},
        {IRByteCode::Store, false},
        {IRByteCode::Jump, false},
        {IRByteCode::Addition, true},
        {IRByteCode::Subtraction, true},
        {IRByteCode::Multiplication, true},
        {IRByteCode::Division, true},
    }));

    REQUIRE(Optimizer::isBinaryOperation(testInput) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgument", "[tcc][optimizer]")
{
    using Argument = IRStatement::Argument;

    auto [testInput, expected] = GENERATE(table<Argument, bool>({
        {Argument {"x1"s}, false},
        {Argument {"t1"s}, false},
        {Argument {1U}, true},
        {Argument {42u}, true},
        {Argument {143u}, true},
        {Argument {uint32_t {1111111}}, true},
    }));

    REQUIRE(Optimizer::isConstantArgument(testInput) == expected);
}

TEST_CASE("tcc/optimizer: ConstantArgumentOptional", "[tcc][optimizer]")
{
    using Argument = IRStatement::OptionalArgument;

    SECTION("false")
    {
        auto [testCase] = GENERATE(table<Argument>({
            {Argument {"x1"s}},
            {Argument {"t1"s}},
            {Argument {std::nullopt}},
        }));

        REQUIRE(Optimizer::isConstantArgument(testCase) == false);
    }

    SECTION("true")
    {
        auto [testCase] = GENERATE(table<Argument>({
            {Argument {1U}},
            {Argument {42u}},
            {Argument {143u}},
            {Argument {uint32_t {1111111}}},
        }));

        REQUIRE(Optimizer::isConstantArgument(testCase) == true);
    }
}

TEST_CASE("tcc/optimizer: ConstantStoreExpression", "[tcc][optimizer]")
{
    SECTION("false")
    {
        auto [testCase] = GENERATE(table<IRStatement>({
            {
                IRStatement {
                    .Type        = IRByteCode::Multiplication,
                    .Destination = "t78"s,
                    .First       = 123U,
                    .Second      = 1U,
                },
            },
            {
                IRStatement {
                    .Type        = IRByteCode::Store,
                    .Destination = "t1"s,
                    .First       = "t0"s,
                    .Second      = std::nullopt,
                },
            },
        }));

        REQUIRE(Optimizer::isConstantStoreExpression(testCase) == false);
    }

    SECTION("true")
    {
        auto [testCase] = GENERATE(table<IRStatement>({
            {
                IRStatement {
                    .Type        = IRByteCode::Store,
                    .Destination = "x1"s,
                    .First       = 1U,
                    .Second      = std::nullopt,
                },
            },
        }));

        REQUIRE(Optimizer::isConstantStoreExpression(testCase) == true);
    }
}

TEST_CASE("tcc/optimizer: ConstantBinaryExpression", "[tcc][optimizer]")
{
    auto [testInput, expected] = GENERATE(table<IRStatement, bool>({
        {IRStatement {
             .Type        = IRByteCode::Store,
             .Destination = "t2"s,
             .First       = "t0"s,
             .Second      = std::nullopt,
         },
         false},
        {IRStatement {
             .Type        = IRByteCode::Multiplication,
             .Destination = "t2"s,
             .First       = 1U,
             .Second      = 451U,
         },
         true},
    }));

    REQUIRE(Optimizer::isConstantBinaryExpression(testInput) == expected);
}

TEST_CASE("tcc/optimizer: UnusedStatement", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "x1"s,
            .First       = 1U,
            .Second      = std::nullopt,
        },
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "x2"s,
            .First       = "x1"s,
            .Second      = std::nullopt,
        },
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("tcc/optimizer: DeleteUnusedStatements", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "t0"s,
            .First       = 1U,
            .Second      = std::nullopt,
        },
    };

    REQUIRE(testData.size() == std::size_t {1});
    REQUIRE(Optimizer::DeleteUnusedStatements(testData) == false);
    REQUIRE(testData.size() == std::size_t {0});
}

TEST_CASE("tcc/optimizer: ReplaceVariableIfConstant", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "x1"s,
            .First       = 143U,
            .Second      = std::nullopt,
        },
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "x2"s,
            .First       = "x1"s,
            .Second      = std::nullopt,
        },
        IRStatement {
            .Type        = IRByteCode::Store,
            .Destination = "t0"s,
            .First       = "x1"s,
            .Second      = std::nullopt,
        },
        IRStatement {
            .Type        = IRByteCode::Addition,
            .Destination = "t0"s,
            .First       = 42U,
            .Second      = "x1"s,
        },
    };

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<std::uint32_t>(testData[1].First) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[2].First) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[3].Second.value()) == 143);
}