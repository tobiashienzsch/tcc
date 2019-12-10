/**
 * @file test_lexer.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/lexer.hpp"

TEST_CASE("lexer: Whitespace", "[lexer]")
{
    auto lexer = tcc::Lexer{" "};
    auto token = lexer.GetNextToken();
    REQUIRE(token.type == tcc::SyntaxToken::Type::WhiteSpace);
}

TEST_CASE("lexer: Number", "[lexer]")
{
    auto lexer = tcc::Lexer{"123"};
    auto token = lexer.GetNextToken();
    REQUIRE(token.type == tcc::SyntaxToken::Type::Number);
}

TEST_CASE("lexer: Simple Combination", "[lexer]")
{
    auto lexer = tcc::Lexer{"42 + 123"};

    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Number);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::WhiteSpace);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Plus);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::WhiteSpace);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Number);
    }
}