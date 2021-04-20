#include "parser.hpp"

#include "catch2/catch.hpp"

TEST_CASE("parser: constant", "[parser]") {
  auto const source = GENERATE(as<std::string>(), "42", "143", "1", "0");
  auto p = parser(source);
  auto ast = p.generate_ast();
  CHECK(ast->get_type() == ast_node_type::constant);
  CHECK(ast->get_children().empty());
}

TEST_CASE("parser: binary_expression", "[parser]") {
  SECTION("plus") {
    auto const source = GENERATE(as<std::string>(), "42+2", "143+2");
    auto p = parser(source);
    auto ast = p.generate_ast();
    CHECK(ast->get_type() == ast_node_type::binary_expression);
    CHECK(ast->get_children().at(0)->get_type() == ast_node_type::constant);
    CHECK(ast->get_children().at(1)->get_type() == ast_node_type::op);
    CHECK(ast->get_children().at(1)->get_source() == std::string("+"));
    CHECK(ast->get_children().at(2)->get_type() == ast_node_type::constant);
  }

  SECTION("minus") {
    auto const source = GENERATE(as<std::string>(), "42-2", "143-2");
    auto p = parser(source);
    auto ast = p.generate_ast();
    CHECK(ast->get_type() == ast_node_type::binary_expression);
    CHECK(ast->get_children().at(0)->get_type() == ast_node_type::constant);
    CHECK(ast->get_children().at(1)->get_type() == ast_node_type::op);
    CHECK(ast->get_children().at(1)->get_source() == std::string("-"));
    CHECK(ast->get_children().at(2)->get_type() == ast_node_type::constant);
  }
}