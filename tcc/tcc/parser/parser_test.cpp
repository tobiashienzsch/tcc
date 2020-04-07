/**
 * @file parser_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/parser.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/testing.hpp"

using namespace tcc::parser::testing;
using IteratorType = std::string::const_iterator;

TEST_CASE("tcc/parser: ParserValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(                   //
      as<std::string>{},                      //
      "int main(){return 0;}",                //
      "void foo(){ int x = 1+2*8-9;return;}"  //
  );

  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);
  auto parser = tcc::Parser{errorHandler};

  auto const result = parser.ParseSource(iter, end);
  REQUIRE(result == true);
  REQUIRE(iter == end);
}

TEST_CASE("tcc/parser: ParserInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(          //
      as<std::string>{},             //
      "int int(){return 0;}",        //
      "void foo(){ int x = 1+**2;}"  //
  );

  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);
  auto parser = tcc::Parser{errorHandler};

  auto const result = parser.ParseSource(iter, end);
  REQUIRE(result == false);
}