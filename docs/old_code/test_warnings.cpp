/**
 * @file test_warnings.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/core/core.hpp"

TEST_CASE("core: IgnoreUnused", "[warnings]")
{
    auto const t = 10;
    tcc::IgnoreUnused(t);
}