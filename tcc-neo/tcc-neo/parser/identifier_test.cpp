/**
 * @file test_identifier.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc-neo/parser/identifier.hpp"

#include "catch2/catch.hpp"

TEST_CASE("tcc-neo/parser: Identifier", "[parser]") {
  auto testCase = GENERATE(as<std::string>{},
                           "_test",    //
                           "_TEST_",   //
                           "test",     //
                           "tesT42",   //
                           "test_143"  //
  );

  std::string result;
  tcc::parser::x3::phrase_parse(std::begin(testCase),     //
                                std::end(testCase),       //
                                tcc::parser::identifier,  //
                                tcc::parser::x3::space,   //
                                result                    //
  );

  REQUIRE(testCase == result);
}