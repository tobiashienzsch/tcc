/**
 * @file expression_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser-qi/expression.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser-qi/skipper.hpp"

TEST_CASE("tcc/parser-qi: ExpressionValid", "[tcc][parser][qi]") {
  using namespace client;

  auto testCase = GENERATE(as<std::string>{}, "1+2", "x*9+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);         // Our error handler
  auto expression = parser::Expression<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                    // Our skipper
  auto ast = ast::Expression{};                                      // Our AST

  bool success = phrase_parse(iter, end, expression, skipper, ast);

  REQUIRE(success == true);
}

TEST_CASE("tcc/parser-qi: ExpressionInvalid", "[tcc][parser][qi]") {
  using namespace client;

  auto testCase = GENERATE(as<std::string>{}, "&", "y+");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);         // Our error handler
  auto expression = parser::Expression<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                    // Our skipper
  auto ast = ast::Expression{};                                      // Our AST

  bool success = phrase_parse(iter, end, expression, skipper, ast);

  REQUIRE(success == false);
}