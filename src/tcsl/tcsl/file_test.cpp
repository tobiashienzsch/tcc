/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcsl/file.hpp"

#include "catch2/catch.hpp"

TEST_CASE("tcsl: File default", "[tcsl]")
{
    auto file = tcc::File {};
    CHECK_FALSE(file.exists());
    CHECK(file.loadAsString().empty());
}

TEST_CASE("tcsl: File(string)", "[tcsl]")
{
    auto file = tcc::File {"testfile", true};
    CHECK(file.exists());
    CHECK(file.sizeInBytes() == 0);
    CHECK(file.loadAsString().empty());

    CHECK(file.remove());
    CHECK(file.sizeInBytes() == -1);
    CHECK_FALSE(file.exists());
    CHECK_FALSE(file.remove());
}