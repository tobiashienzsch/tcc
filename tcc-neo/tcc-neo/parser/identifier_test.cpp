/**
 * @file test_identifier.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc-neo/parser/identifier.hpp"

TEST_CASE("tcc-neo/parser: Identifier", "[parser]")
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