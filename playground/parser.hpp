#ifndef TCC_PLAYGROUND_PARSER_HPP
#define TCC_PLAYGROUND_PARSER_HPP

#include "ast.hpp"
#include "lexer.hpp"
#include "parse_context.hpp"

#include <algorithm>
#include <memory>

class parser {
public:
  explicit parser(std::string_view text) {
    auto lex = lexer{context_, text};
    auto token = lex.next_token();
    while (token.type != syntax_token_type::end_of_file) {
      if (token.type != syntax_token_type::whitespace &&
          token.type != syntax_token_type::bad_token) {
        tokens_.push_back(token);
      }
      token = lex.next_token();
    }
    tokens_.push_back(token);
  }

  [[nodiscard]] auto generate_ast() -> std::unique_ptr<ast_node> {
    auto exp = parse_expression();
    auto eof = match_token(syntax_token_type::end_of_file);
    return exp;
  }

  auto print_diagnostics(std::ostream &out) const -> void {
    context_.print_errors(out);
  }

  [[nodiscard]] auto get_tokens() const noexcept
      -> std::vector<syntax_token> const & {
    return tokens_;
  }

private:
  [[nodiscard]] auto parse_expression(int parent_precedence = 0)
      -> std::unique_ptr<ast_node> {
    auto left = std::unique_ptr<ast_node>();
    auto unary_precedence = get_unary_opprecedence(current().type);
    if (unary_precedence != 0 && unary_precedence >= parent_precedence) {
      auto op_token = next_token();
      auto operand = parse_expression(unary_precedence);
      left = std::make_unique<ast_unary_expr>(op_token, std::move(operand));
    } else {
      left = parse_primary_expression();
    }

    while (true) {
      auto const precedence = get_binary_opprecedence(current().type);
      if (precedence == 0 || precedence <= parent_precedence) {
        break;
      }

      auto op_token = next_token();
      auto right = parse_expression(precedence);
      left = std::make_unique<ast_binary_expr>(std::move(left), op_token,
                                               std::move(right));
    }

    return left;
  }

  [[nodiscard]] auto parse_primary_expression() -> std::unique_ptr<ast_node> {

    if (current().type == syntax_token_type::open_brace) {
      auto left = next_token();
      auto expression = parse_expression();
      auto right = match_token(syntax_token_type::close_brace);
      return std::make_unique<ast_braced_expr>(left, std::move(expression),
                                               right);
    }

    auto number_token = match_token(syntax_token_type::literal_integer);
    return std::make_unique<ast_constant>(number_token);
  }

  [[nodiscard]] auto match_token(syntax_token_type type) noexcept
      -> syntax_token {
    if (current().type == type) {
      return next_token();
    }

    context_.add_error(
        fmt::format("at {0} unexpected token <{1}>, expected <{2}>",
                    current().position, current().type, type));
    return syntax_token{type, current().position, ""};
  }

  [[nodiscard]] auto next_token() noexcept -> syntax_token {
    auto cur = current();
    position_++;
    return cur;
  }

  [[nodiscard]] auto peek(size_t offset) const noexcept -> syntax_token {
    auto const index = position_ + offset;
    if (index >= tokens_.size()) {
      return tokens_.back();
    }
    return tokens_.at(index);
  }

  [[nodiscard]] auto current() const noexcept -> syntax_token {
    return peek(0);
  }

  parse_context context_{};
  std::size_t position_{};
  std::vector<syntax_token> tokens_{};
};

#endif // TCC_PLAYGROUND_PARSER_HPP
