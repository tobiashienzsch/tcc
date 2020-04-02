/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/parser/statement.hpp"

#include <sstream>

#include "catch2/catch.hpp"
#include "tcc/parser/config.hpp"

using boost::spirit::x3::with;
using boost::spirit::x3::ascii::space;
using tcc::parser::error_handler_type;
using tcc::parser::iterator_type;

namespace {
class NullBuffer : public std::streambuf {
 public:
  int overflow(int c) { return c; }
};

NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);
}  // namespace

TEST_CASE("tcc/parser: Statement", "[tcc][parser]") {
  auto testCase = GENERATE(as<std::string>{}, "auto x = 0;", "x = y+z;");

  iterator_type iter(testCase.begin());
  iterator_type end(testCase.end());
  error_handler_type error_handler(iter, end, null_stream);

  auto const grammar = tcc::parser::Statement_type("Statement");
  auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[grammar];

  tcc::ast::StatementList ast;
  bool success = tcc::parser::x3::phrase_parse(iter, end, parser, space, ast);
  REQUIRE(success == true);
}

TEST_CASE("tcc/parser: StatementInvalid", "[tcc][parser]") {
  auto testCase = GENERATE(as<std::string>{}, "auto 8x = 0;", "x = y+z");

  iterator_type iter(testCase.begin());
  iterator_type end(testCase.end());
  error_handler_type error_handler(iter, end, null_stream);

  auto const grammar = tcc::parser::Statement_type("Statement");
  auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[grammar];

  tcc::ast::StatementList ast;
  bool success = tcc::parser::x3::phrase_parse(iter, end, parser, space, ast);
  REQUIRE(success == false);
}

TEST_CASE("tcc/parser: ReturnStatementValid", "[tcc][parser]") {
  auto const testCase = std::string{"auto x= 1;\n return x+y;"};
  iterator_type iter(testCase.begin());
  iterator_type end(testCase.end());
  error_handler_type error_handler(iter, end, std::cerr);

  auto const grammar = tcc::parser::Statement_type("Statement");
  auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[grammar];

  tcc::ast::StatementList ast;
  bool success = tcc::parser::x3::phrase_parse(iter, end, parser, space, ast);
  REQUIRE(success == true);
}

TEST_CASE("tcc/parser: ReturnStatementInvalid", "[tcc][parser]") {
  auto testCase = GENERATE(as<std::string>{}, "return $", "return false");
  iterator_type iter(testCase.begin());
  iterator_type end(testCase.end());
  error_handler_type error_handler(iter, end, null_stream);

  auto const grammar = tcc::parser::Statement_type("Statement");
  auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[grammar];

  tcc::ast::StatementList ast;
  bool success = tcc::parser::x3::phrase_parse(iter, end, parser, space, ast);
  REQUIRE(success == false);
}