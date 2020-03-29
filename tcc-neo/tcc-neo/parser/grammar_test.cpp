/**
 * @file test_grammar.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc-neo/parser/grammar.hpp"

TEST_CASE("tcc-neo/grammar: Identifier", "[grammar]")
{
    auto testCases = std::vector<std::string> {
        std::string {"_test"},     //
        std::string {"_TEST_"},    //
        std::string {"test"},      //
        std::string {"tesT42"},    //
        std::string {"test_143"},  //
    };

    for (auto const& test : testCases)
    {
        std::string result;
        tcc::parser::x3::phrase_parse(std::begin(test),         //
                                      std::end(test),           //
                                      tcc::parser::identifier,  //
                                      tcc::parser::x3::space,   //
                                      result                    //
        );

        REQUIRE(result == test);
    }
}

TEST_CASE("tcc-neo/grammar: PackageDeclaration", "[grammar]")
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

TEST_CASE("tcc-neo/grammar: PackageDefinition", "[grammar]")
{
    auto const input = std::string(R"(
        package main;
        auto main() -> int {}
    )");

    tcc::parser::packageDefinition_t result;
    tcc::parser::x3::phrase_parse(std::begin(input),               //
                                  std::end(input),                 //
                                  tcc::parser::packageDefinition,  //
                                  tcc::parser::x3::space,          //
                                  result                           //
    );

    REQUIRE(std::get<0>(result) == "main");
    REQUIRE(std::get<1>(result)[0] == "main");
    REQUIRE(std::get<1>(result)[1] == "int");
}