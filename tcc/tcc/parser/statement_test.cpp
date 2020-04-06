/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/parser/statement.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/skipper.hpp"

TEST_CASE("tcc/parser: StatementValid", "[tcc][parser][qi]") {
  using namespace tcc;

  auto testCase = GENERATE(as<std::string>{}, "int x = 0;", "x = y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);       // Our error handler
  auto statement = parser::Statement<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                  // Our skipper
  auto ast = ast::StatementList{};                                 // Our AST

  bool success = phrase_parse(iter, end, statement, skipper, ast);

  REQUIRE(success == true);
}

TEST_CASE("tcc/parser: StatementInvalid", "[tcc][parser][qi]") {
  using namespace tcc;

  auto testCase = GENERATE(as<std::string>{}, "auto x = 0;", "= y+z;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);       // Our error handler
  auto statement = parser::Statement<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                  // Our skipper
  auto ast = ast::StatementList{};                                 // Our AST

  bool success = phrase_parse(iter, end, statement, skipper, ast);

  REQUIRE(success == false);
}
TEST_CASE("tcc/parser: ReturnStatementValid", "[tcc][parser][qi]") {
  using namespace tcc;

  auto testCase = GENERATE(as<std::string>{}, "return;", "return x;", "return 1+2;");

  using IteratorType = std::string::const_iterator;
  IteratorType iter = testCase.begin();
  IteratorType end = testCase.end();

  auto errorHandler = ErrorHandler<IteratorType>(iter, end);       // Our error handler
  auto statement = parser::Statement<IteratorType>(errorHandler);  // Our parser
  auto skipper = parser::Skipper<IteratorType>{};                  // Our skipper
  auto ast = ast::ReturnStatement{};                               // Our AST

  bool success = phrase_parse(iter, end, statement.ReturnStatement, skipper, ast);

  REQUIRE(success == true);
}
