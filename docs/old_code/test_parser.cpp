/**
 * @file test_parser.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler_old/parser.hpp"

TEST_CASE("parser: Simple", "[parser]")
{
    auto parser = tcc::Parser {"auto main(){}"};
    // auto token  = parser.Parse();
    // REQUIRE(token.type == tcc::SyntaxToken::Type::EndOfFile);
}