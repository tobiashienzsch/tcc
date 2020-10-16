#ifndef TCC_PLAYGROUND_LEXER_HPP
#define TCC_PLAYGROUND_LEXER_HPP

#include "parse_context.hpp"
#include "token.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cctype>
#include <cstddef>
#include <string>

class Lexer {
public:
  Lexer(ParseContext &ctx, std::string text)
      : context_{ctx}, text_{std::move(text)} {}

  auto NextToken() -> SyntaxToken {
    if (position_ >= text_.size()) {
      return SyntaxToken{
          .Type = SyntaxTokenType::EndOfFile,
          .Position = position_,
          .Text = std::string(1, '\0'),
      };
    }
    // Whitespace
    if (std::isspace(current()) != 0) {
      auto const start = position_;
      while (std::isspace(current()) != 0) {
        advance();
      }

      auto const length = position_ - start;

      return SyntaxToken{
          .Type = SyntaxTokenType::Whitespace,
          .Position = start,
          .Text = text_.substr(start, length),
      };
    }
    // LiteralInteger
    if (std::isdigit(current()) != 0) {
      auto const start = position_;
      while (std::isdigit(current()) == 1) {
        advance();
      }

      auto const length = position_ - start;
      return SyntaxToken{
          .Type = SyntaxTokenType::LiteralInteger,
          .Position = start,
          .Text = text_.substr(start, length),
      };
    }

    if (current() == '+') {
      return {SyntaxTokenType::Plus, position_++, "+"};
    }

    if (current() == '-') {
      return {SyntaxTokenType::Minus, position_++, "-"};
    }

    if (current() == '*') {
      return {SyntaxTokenType::Star, position_++, "*"};
    }

    if (current() == '/') {
      return {SyntaxTokenType::Slash, position_++, "/"};
    }

    if (current() == '(') {
      return {SyntaxTokenType::OpenBrace, position_++, "("};
    }

    if (current() == ')') {
      return {SyntaxTokenType::CloseBrace, position_++, ")"};
    }

    context_.AddError(fmt::format("bad character input: {0}", current()));

    return SyntaxToken{
        .Type = SyntaxTokenType::BadToken,
        .Position = position_++,
        .Text = text_.substr(position_ - 1, 1),
    };
  }

private:
  [[nodiscard]] auto current() const noexcept -> char {
    return position_ >= text_.size() ? '\0' : text_[position_];
  }

  auto advance() noexcept -> std::size_t { return ++position_; }

  ParseContext &context_;
  std::string text_;
  std::size_t position_{0};
};
#endif // TCC_PLAYGROUND_LEXER_HPP
