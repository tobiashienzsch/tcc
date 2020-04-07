/**
 * @file parser_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/parser.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/error_handler.hpp"

using IteratorType = std::string::const_iterator;

TEST_CASE("tcc/parser: ParserValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "int main(){return 0;}", "void foo(){ int x = 1+2*8-9;return;}");

  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();
  auto errorHandler = tcc::ErrorHandler<IteratorType>{iter, end}; 
  auto parser = tcc::Parser{errorHandler};

  auto const result = parser.ParseSource(iter, end);
  REQUIRE(result == true);
  REQUIRE(iter == end);
}

TEST_CASE("tcc/parser: ParserInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "int int(){return 0;}", "void foo(){ int x = 1+**2;}");

  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();
  auto errorHandler = tcc::ErrorHandler<IteratorType>{iter, end}; 
  auto parser = tcc::Parser{errorHandler};

  auto const result = parser.ParseSource(iter, end);
  REQUIRE(result == false);
}