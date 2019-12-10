/**
 * @file test_lexer.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/lexer.hpp"

TEST_CASE("lexer: EndOfFile", "[lexer]")
{
    auto lexer = tcc::Lexer{""};
    auto token = lexer.GetNextToken();
    REQUIRE(token.type == tcc::SyntaxToken::Type::EndOfFile);
}

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

TEST_CASE("lexer: Identifier", "[lexer]")
{
    auto lexer = tcc::Lexer{"int main"};

    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Identifier);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::WhiteSpace);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Identifier);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::EndOfFile);
    }
}

TEST_CASE("lexer: Special Characters", "[lexer]")
{
    auto lexer = tcc::Lexer{"+-*/%&#!"};

    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Plus);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Minus);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Star);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Slash);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Percent);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::And);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::Hash);
    }
    {
        auto token = lexer.GetNextToken();
        REQUIRE(token.type == tcc::SyntaxToken::Type::ExclamationMark);
    }
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
