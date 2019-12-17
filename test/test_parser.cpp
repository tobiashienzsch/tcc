/**
 * @file test_parser.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/parser.hpp"

TEST_CASE("parser: Simple", "[parser]")
{
    auto parser = tcc::Parser{"auto main(){}"};
    // auto token  = parser.Parse();
    // REQUIRE(token.type == tcc::SyntaxToken::Type::EndOfFile);
}