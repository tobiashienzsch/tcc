#ifndef TCC_PLAYGROUND_PARSER_HPP
#define TCC_PLAYGROUND_PARSER_HPP

#include "ast.hpp"
#include "lexer.hpp"
#include "parse_context.hpp"

#include <algorithm>
#include <memory>

class Parser {
public:
  Parser(std::string text) {
    auto lexer = Lexer{context_, std::move(text)};
    auto token = lexer.NextToken();
    for (; token.Type != SyntaxTokenType::EndOfFile;
         token = lexer.NextToken()) {
      if (token.Type != SyntaxTokenType::Whitespace &&
          token.Type != SyntaxTokenType::BadToken) {
        tokens_.push_back(token);
      }
    }
  }

  [[nodiscard]] auto Parse() -> std::unique_ptr<ASTNode> {
    auto exp = parseTerm();
    auto eof = match(SyntaxTokenType::EndOfFile);
    return exp;
  }

  auto PrintDiagnostics(std::ostream &out) { context_.PrintErrors(out); }

private:
  [[nodiscard]] auto parseTerm() -> std::unique_ptr<ASTNode> {
    auto lhs = parseFactor();

    while (current().Type == SyntaxTokenType::Plus ||
           current().Type == SyntaxTokenType::Minus) {
      auto op = nextToken();
      auto rhs = parseFactor();
      lhs = std::make_unique<ASTBinaryExpr>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
  }

  [[nodiscard]] auto parseFactor() -> std::unique_ptr<ASTNode> {
    auto lhs = parsePrimaryExpression();

    while (current().Type == SyntaxTokenType::Star ||
           current().Type == SyntaxTokenType::Slash) {
      auto op = nextToken();
      auto rhs = parsePrimaryExpression();
      lhs = std::make_unique<ASTBinaryExpr>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
  }

  [[nodiscard]] auto parsePrimaryExpression() -> std::unique_ptr<ASTNode> {
    auto numberToken = match(SyntaxTokenType::LiteralInteger);
    return std::make_unique<ASTConstantExpr>(numberToken);
  }

  [[nodiscard]] auto match(SyntaxTokenType type) noexcept -> SyntaxToken {
    if (current().Type == type) {
      return nextToken();
    }

    context_.AddError(
        fmt::format("at {0} unexpected token <{1}>, expected <{2}>",
                    current().Position, current().Type, type));
    return SyntaxToken{type, current().Position, ""};
  }

  [[nodiscard]] auto nextToken() noexcept -> SyntaxToken {
    auto cur = current();
    position_++;
    return cur;
  }

  [[nodiscard]] auto peek(size_t offset) const noexcept -> SyntaxToken {
    auto const index = position_ + offset;
    auto const maxIndex = std::max<size_t>(tokens_.size() - 1, 0);
    return tokens_.at(std::min(index, maxIndex));
  }

  [[nodiscard]] auto current() const noexcept -> SyntaxToken { return peek(0); }

  ParseContext context_{};
  std::size_t position_{};
  std::vector<SyntaxToken> tokens_{};
};

#endif // TCC_PLAYGROUND_PARSER_HPP
