/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcsl/file.hpp"

#include "catch2/catch.hpp"

TEST_CASE("tcsl: File default", "[tcsl]")
{
    auto file = tcc::File {};
    CHECK_FALSE(file.Exists());
    CHECK(file.LoadAsString().empty());
}

TEST_CASE("tcsl: File(string)", "[tcsl]")
{
    auto file = tcc::File {"testfile"};
    CHECK(file.Exists());
    CHECK(file.SizeInBytes() == 0);
    CHECK(file.LoadAsString().empty());

    CHECK(file.Remove());
    CHECK(file.SizeInBytes() == -1);
    CHECK_FALSE(file.Exists());
    CHECK_FALSE(file.Remove());
}