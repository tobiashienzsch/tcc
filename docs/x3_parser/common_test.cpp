/**
 * @file common_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/parser/common.hpp"

#include <sstream>

#include "catch2/catch.hpp"

TEST_CASE("tcc/parser: Identifier", "[tcc][parser]") {
  auto testCase = GENERATE(as<std::string>{}, "_test", "_TEST_", "test", "tesT42", "test_143");

  std::string result;
  auto const success = tcc::parser::x3::phrase_parse(std::begin(testCase), std::end(testCase), tcc::parser::identifier,
                                                     tcc::parser::x3::space, result);

  REQUIRE(success == true);
  REQUIRE(testCase == result);
}

TEST_CASE("tcc/parser: IdentifierInvalid", "[tcc][parser]") {
  auto testCase = GENERATE(as<std::string>{}, "8test", "89_Test", "%Test", "/test");

  std::string result;
  auto const success = tcc::parser::x3::phrase_parse(std::begin(testCase), std::end(testCase), tcc::parser::identifier,
                                                     tcc::parser::x3::space, result);

  REQUIRE(success == false);
}