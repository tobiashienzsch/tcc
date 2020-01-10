/**
 * @file test_warnings.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/core.hpp"

TEST_CASE("core: IgnoreUnused", "[warnings]")
{
    auto const t = 10;
    tcc::IgnoreUnused(t);
}