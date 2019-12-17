/**
 * @file test_parser_x3.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/parser_x3.hpp"

TEST_CASE("parser_x3: Identifier", "[parser_x3]")
{
    auto testCases = std::vector<std::string>{
        std::string{"_test"},     //
        std::string{"_TEST_"},    //
        std::string{"test"},      //
        std::string{"tesT42"},    //
        std::string{"test_143"},  //
    };

    for (auto const& test : testCases)
    {
        std::string result;
        tcc::parser::x3::phrase_parse(std::begin(test),  //
                                      std::end(test),    //
                                      // START GRAMMER
                                      tcc::parser::identifier,  //
                                      // END GRAMMER
                                      tcc::parser::x3::space,  //
                                      result                   //
        );

        REQUIRE(result == test);
    }
}