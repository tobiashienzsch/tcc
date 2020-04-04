#include "tcc/parser/config.hpp"
#include "tcc/parser/expression_def.hpp"

namespace tcc::parser {
void add_keywords() {
  static bool once = false;
  if (once) return;
  once = true;

  LogicalOperators.add("&&", ast::OpToken::And);
  LogicalOperators.add("||", ast::OpToken::Or);

  EqualityOperators.add("==", ast::OpToken::Equal);
  EqualityOperators.add("!=", ast::OpToken::NotEqual);

  RelationalOperators.add("<", ast::OpToken::Less);
  RelationalOperators.add("<=", ast::OpToken::LessEqual);
  RelationalOperators.add(">", ast::OpToken::Greater);
  RelationalOperators.add(">=", ast::OpToken::GreaterEqual);

  AdditiveOperators.add("+", ast::OpToken::Plus);
  AdditiveOperators.add("-", ast::OpToken::Minus);

  MultiplicativeOperators.add("*", ast::OpToken::Times);
  MultiplicativeOperators.add("/", ast::OpToken::Divide);

  UnaryOperators.add("+", ast::OpToken::Positive);
  UnaryOperators.add("-", ast::OpToken::Negative);
  UnaryOperators.add("!", ast::OpToken::Not);

  keywords.add("auto");
  keywords.add("true");
  keywords.add("false");
  keywords.add("if");
  keywords.add("else");
  keywords.add("while");
  keywords.add("return");
}

BOOST_SPIRIT_INSTANTIATE(Expression_type, iterator_type, context_type)
}  // namespace tcc::parser
namespace tcc {
parser::Expression_type const& Expression() {
  parser::add_keywords();
  return parser::Expression;
}
}  // namespace tcc