/**
 * @file test_package.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc-neo/parser/package.hpp"

TEST_CASE("tcc-neo/parser: PackageDeclaration", "[parser]")
{
    struct TestCase
    {
        std::string input;
        std::string expected;
    };

    auto testCases = std::vector<TestCase> {
        {std::string {"package main;"}, std::string {"main"}},            //
        {std::string {"package math;"}, std::string {"math"}},            //
        {std::string {"package math_test;"}, std::string {"math_test"}},  //
        {std::string {"package tobi_est;"}, std::string {"tobi_est"}},    //
        {std::string {"package fmt8;"}, std::string {"fmt8"}},            //
        {std::string {";"}, std::string {""}},                            //

    };

    for (auto const& test : testCases)
    {
        tcc::parser::packageDeclaration_t result;
        tcc::parser::x3::phrase_parse(std::begin(test.input),           //
                                      std::end(test.input),             //
                                      tcc::parser::packageDeclaration,  //
                                      tcc::parser::x3::space,           //
                                      result                            //
        );

        REQUIRE(result == test.expected);
    }
}
