/**
 * @file test_warnings.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/tcc.hpp"

TEST_CASE("core: IgnoreUnused", "[warnings]")
{
    auto const t = 10;
    tcc::IgnoreUnused(t);
}