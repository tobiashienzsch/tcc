/**
 * @file test_lexer.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/lexer.hpp"

TEST_CASE("compiler: Lexer", "[lexer]")
{
    auto const source = std::string("10");
    auto lexer        = tcc::Lexer{source};
    REQUIRE(true == true);
}