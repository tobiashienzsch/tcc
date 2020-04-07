/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/statement.hpp"

#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/skipper.hpp"
#include "tcc/parser/testing.hpp"

using namespace tcc::parser::testing;

TEST_CASE("tcc/parser: StatementValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "int x = 0;", "x = y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto grammar = tcc::parser::Statement<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::StatementList{};
  REQUIRE(phrase_parse(iter, end, grammar, skipper, ast) == true);
}

TEST_CASE("tcc/parser: StatementInvalid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "auto x = 0;", "= y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto statement = tcc::parser::Statement<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::StatementList{};

  REQUIRE(phrase_parse(iter, end, statement, skipper, ast) == false);
}

TEST_CASE("tcc/parser: ReturnStatementValid", "[tcc][parser][qi]") {
  auto testCase = GENERATE(as<std::string>{}, "return;", "return x;", "return 1+2;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  auto errorHandler = tcc::ErrorHandler<IteratorType>(iter, end, null_stream);

  auto statement = tcc::parser::Statement<IteratorType>(errorHandler);
  auto skipper = tcc::parser::Skipper<IteratorType>{};
  auto ast = tcc::ast::ReturnStatement{};

  REQUIRE(phrase_parse(iter, end, statement.ReturnStatement, skipper, ast) == true);
}
