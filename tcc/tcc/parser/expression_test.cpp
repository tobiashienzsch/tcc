/**
 * @file expression_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/expression.hpp"

#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/skipper.hpp"
#include "tcc/parser/testing.hpp"

using namespace tcc::parser::testing;

TEST_CASE("tcc/parser: IdentifierValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "test", "TEST", "foo8", "test_foo");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto expression = tcc::parser::Expression<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = std::string{};

  REQUIRE(phrase_parse(iter, end, expression.Identifier, skipper, ast) == true);
}

TEST_CASE("tcc/parser: IdentifierInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "#y", "8foo", "@foo");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto expression = tcc::parser::Expression<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = std::string{};

  REQUIRE(phrase_parse(iter, end, expression.Identifier, skipper, ast) == false);
}
TEST_CASE("tcc/parser: ExpressionValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "1+2", "x*9+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto expression = tcc::parser::Expression<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::Expression{};

  REQUIRE(phrase_parse(iter, end, expression, skipper, ast) == true);
}

TEST_CASE("tcc/parser: ExpressionInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "&", "y+");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto expression = tcc::parser::Expression<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::Expression{};

  REQUIRE(phrase_parse(iter, end, expression, skipper, ast) == false);
}