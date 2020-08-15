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
                    .type        = IRByteCode::Multiplication,
                    .destination = "t78"s,
                    .first       = 123U,
                    .second      = 1U,
                },
            },
            {
                IRStatement {
                    .type        = IRByteCode::Store,
                    .destination = "t1"s,
                    .first       = "t0"s,
                    .second      = std::nullopt,
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
                    .type        = IRByteCode::Store,
                    .destination = "x1"s,
                    .first       = 1U,
                    .second      = std::nullopt,
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
             .type        = IRByteCode::Store,
             .destination = "t2"s,
             .first       = "t0"s,
             .second      = std::nullopt,
         },
         false},
        {IRStatement {
             .type        = IRByteCode::Multiplication,
             .destination = "t2"s,
             .first       = 1U,
             .second      = 451U,
         },
         true},
    }));

    REQUIRE(Optimizer::isConstantBinaryExpression(testInput) == expected);
}

TEST_CASE("tcc/optimizer: UnusedStatement", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {
            .type        = IRByteCode::Store,
            .destination = "x1"s,
            .first       = 1U,
            .second      = std::nullopt,
        },
        IRStatement {
            .type        = IRByteCode::Store,
            .destination = "x2"s,
            .first       = "x1"s,
            .second      = std::nullopt,
        },
    };

    REQUIRE(Optimizer::IsUnusedStatement(testData.at(0), testData) == false);
    REQUIRE(Optimizer::IsUnusedStatement(testData.at(1), testData) == true);
}

TEST_CASE("tcc/optimizer: DeleteUnusedStatements", "[tcc][optimizer]")
{
    auto testData = tcc::IRStatementList {
        IRStatement {
            .type        = IRByteCode::Store,
            .destination = "t0"s,
            .first       = 1U,
            .second      = std::nullopt,
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
            .type        = IRByteCode::Store,
            .destination = "x1"s,
            .first       = 143U,
            .second      = std::nullopt,
        },
        IRStatement {
            .type        = IRByteCode::Store,
            .destination = "x2"s,
            .first       = "x1"s,
            .second      = std::nullopt,
        },
        IRStatement {
            .type        = IRByteCode::Store,
            .destination = "t0"s,
            .first       = "x1"s,
            .second      = std::nullopt,
        },
        IRStatement {
            .type        = IRByteCode::Addition,
            .destination = "t0"s,
            .first       = 42U,
            .second      = "x1"s,
        },
    };

    REQUIRE(Optimizer::ReplaceVariableIfConstant(testData.at(0), testData) == true);
    REQUIRE(std::get<std::uint32_t>(testData[1].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[2].first) == 143);
    REQUIRE(std::get<std::uint32_t>(testData[3].second.value()) == 143);
}