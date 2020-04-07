/**
 * @file function_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/function.hpp"

#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/skipper.hpp"
#include "tcc/parser/testing.hpp"

using namespace tcc::parser::testing;

TEST_CASE("tcc/parser: FunctionValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "void foo(){ int foo = 5; }", "int foo(){return 1;}");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto function = tcc::parser::Function<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::FunctionList{};

  REQUIRE(phrase_parse(iter, end, function, skipper, ast) == true);
}

TEST_CASE("tcc/parser: FunctionInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "int int(){}", "void void(){}");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto function = tcc::parser::Function<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::FunctionList{};

  REQUIRE(phrase_parse(iter, end, function, skipper, ast) == false);
}