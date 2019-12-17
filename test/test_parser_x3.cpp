/**
 * @file test_parser_x3.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/parser_x3.hpp"

TEST_CASE("parser_x3: Simple", "[parser_x3]")
{
    auto parser = std::string{"auto main(){}"};
    // auto token  = parser.Parse();
    // REQUIRE(token.type == tcc::SyntaxToken::Type::EndOfFile);
}