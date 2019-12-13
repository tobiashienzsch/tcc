/**
 * @file test_lexer.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/ast.hpp"

TEST_CASE("ast: ConstantExpression", "[ast]")
{
    auto const val        = 5;
    auto const expression = tcc::ConstantExpression(val);
    auto const assembly   = expression.GetAssembly();
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == val);
}

TEST_CASE("ast: BinaryExpression", "[ast]")
{
    tcc::BinaryExpression expression = {
        new tcc::ConstantExpression(4),   //
        new tcc::ConstantExpression(2),   //
        tcc::BinaryExpression::Type::Add  //
    };

    auto const assembly = expression.GetAssembly();
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 4);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(3) == 2);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(4)) == tcc::ByteCode::IADD);
}