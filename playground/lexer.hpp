#ifndef TCC_PLAYGROUND_LEXER_HPP
#define TCC_PLAYGROUND_LEXER_HPP

#include "parse_context.hpp"
#include "token.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cctype>
#include <cstddef>
#include <string_view>

class lexer {
public:
  lexer(parse_context &ctx, std::string_view text)
      : context_{ctx}, text_{text} {}

  auto next_token() -> syntax_token {
    if (position_ >= text_.size()) {
      return syntax_token{
          .type = syntax_token_type::end_of_file,
          .position = position_,
          .text = std::string(1, '\0'),
      };
    }
    // whitespace
    if (std::isspace(current()) != 0) {
      auto const start = position_;
      while (std::isspace(current()) != 0) {
        advance();
      }

      auto const length = position_ - start;

      return syntax_token{
          .type = syntax_token_type::whitespace,
          .position = start,
          .text = std::string{text_.substr(start, length)},
      };
    }
    // literal_integer
    if (std::isdigit(current()) != 0) {
      auto const start = position_;
      while (std::isdigit(current()) == 1) {
        advance();
      }

      auto const length = position_ - start;
      return syntax_token{
          .type = syntax_token_type::literal_integer,
          .position = start,
          .text = std::string{text_.substr(start, length)},
      };
    }

    if (current() == '+') {
      return {syntax_token_type::plus, position_++, "+"};
    }

    if (current() == '-') {
      return {syntax_token_type::minus, position_++, "-"};
    }

    if (current() == '*') {
      return {syntax_token_type::star, position_++, "*"};
    }

    if (current() == '/') {
      return {syntax_token_type::slash, position_++, "/"};
    }

    if (current() == '(') {
      return {syntax_token_type::open_brace, position_++, "("};
    }

    if (current() == ')') {
      return {syntax_token_type::close_brace, position_++, ")"};
    }

    context_.add_error(fmt::format("bad character input: {0}", current()));

    return syntax_token{
        .type = syntax_token_type::bad_token,
        .position = position_++,
        .text = std::string{text_.substr(position_ - 1, 1)},
    };
  }

private:
  [[nodiscard]] auto current() const noexcept -> char {
    return position_ >= text_.size() ? '\0' : text_[position_];
  }

  auto advance() noexcept -> std::size_t { return ++position_; }

  parse_context &context_;
  std::string_view text_;
  std::size_t position_{0};
};
#endif // TCC_PLAYGROUND_LEXER_HPP
