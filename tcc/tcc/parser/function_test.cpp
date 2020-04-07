/**
 * @file function_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/function.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/skipper.hpp"

TEST_CASE("tcc/parser: FunctionValid", "[tcc][parser][qi]") {
  using namespace tcc;

  auto testCase = GENERATE(as<std::string>{}, "int x = 0;", "x = y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);       // Our error handler
  auto function = parser::Function<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                  // Our skipper
  auto ast = ast::FunctionList{};                                 // Our AST

  bool success = phrase_parse(iter, end, function, skipper, ast);

  REQUIRE(success == true);
}

TEST_CASE("tcc/parser: FunctionInvalid", "[tcc][parser][qi]") {
  using namespace tcc;

  auto testCase = GENERATE(as<std::string>{}, "auto x = 0;", "= y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);       // Our error handler
  auto function = parser::Function<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                  // Our skipper
  auto ast = ast::FunctionList{};                                 // Our AST

  bool success = phrase_parse(iter, end, function, skipper, ast);

  REQUIRE(success == false);
}