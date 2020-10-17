#include "parser.hpp"

#include "catch2/catch.hpp"

TEST_CASE("parser: Constant", "[parser]") {
  auto const source = GENERATE(as<std::string>(), "42", "143", "1", "0");
  auto parser = Parser(source);
  auto ast = parser.GenerateAST();
  CHECK(ast->GetType() == ASTNodeType::Constant);
  CHECK(ast->GetChildren().empty());
}

TEST_CASE("parser: BinaryExpr", "[parser]") {
  SECTION("plus") {
    auto const source = GENERATE(as<std::string>(), "42+2", "143+2");
    auto parser = Parser(source);
    auto ast = parser.GenerateAST();
    CHECK(ast->GetType() == ASTNodeType::BinaryExpression);
    CHECK(ast->GetChildren().at(0)->GetType() == ASTNodeType::Constant);
    CHECK(ast->GetChildren().at(1)->GetType() == ASTNodeType::Operator);
    CHECK(ast->GetChildren().at(1)->GetSource() == std::string("+"));
    CHECK(ast->GetChildren().at(2)->GetType() == ASTNodeType::Constant);
  }

  SECTION("minus") {
    auto const source = GENERATE(as<std::string>(), "42-2", "143-2");
    auto parser = Parser(source);
    auto ast = parser.GenerateAST();
    CHECK(ast->GetType() == ASTNodeType::BinaryExpression);
    CHECK(ast->GetChildren().at(0)->GetType() == ASTNodeType::Constant);
    CHECK(ast->GetChildren().at(1)->GetType() == ASTNodeType::Operator);
    CHECK(ast->GetChildren().at(1)->GetSource() == std::string("-"));
    CHECK(ast->GetChildren().at(2)->GetType() == ASTNodeType::Constant);
  }
}