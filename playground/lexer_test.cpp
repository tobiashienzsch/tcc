#include "lexer.hpp"

#include "catch2/catch.hpp"

TEST_CASE("lexer: Operator", "[lexer]") {
  auto const source = GENERATE(as<std::string>(), "+", "-", "*", "/");
  auto ctx = ParseContext{};

  CHECK(Lexer(ctx, "+").NextToken().Type == SyntaxTokenType::Plus);
  CHECK(Lexer(ctx, "-").NextToken().Type == SyntaxTokenType::Minus);
  CHECK(Lexer(ctx, "*").NextToken().Type == SyntaxTokenType::Star);
  CHECK(Lexer(ctx, "/").NextToken().Type == SyntaxTokenType::Slash);
  CHECK(Lexer(ctx, "(").NextToken().Type == SyntaxTokenType::OpenBrace);
  CHECK(Lexer(ctx, ")").NextToken().Type == SyntaxTokenType::CloseBrace);

  CHECK_FALSE(ctx.HasErrors());
}

TEST_CASE("lexer: NextToken", "[lexer]") {
  SECTION("No spaces") {
    auto const source = GENERATE(as<std::string>(), "42+2", "12+3", "99+0");
    auto ctx = ParseContext{};
    auto lexer = Lexer(ctx, source);

    auto token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::LiteralInteger);
    CHECK(token.Position == 0);
    CHECK(token.Text.size() == 2);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::Plus);
    CHECK(token.Position == 2);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::LiteralInteger);
    CHECK(token.Position == 3);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::EndOfFile);
    CHECK(token.Position == 4);
    CHECK(token.Text.size() == 1);

    CHECK_FALSE(ctx.HasErrors());
  }

  SECTION("With spaces") {
    auto const source = GENERATE(as<std::string>(), " 42 + 2", " 12 + 3");
    auto ctx = ParseContext{};
    auto lexer = Lexer(ctx, source);

    auto token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::Whitespace);
    CHECK(token.Position == 0);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::LiteralInteger);
    CHECK(token.Position == 1);
    CHECK(token.Text.size() == 2);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::Whitespace);
    CHECK(token.Position == 3);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::Plus);
    CHECK(token.Position == 4);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::Whitespace);
    CHECK(token.Position == 5);
    CHECK(token.Text.size() == 1);

    token = lexer.NextToken();
    CHECK(token.Type == SyntaxTokenType::LiteralInteger);
    CHECK(token.Position == 6);
    CHECK(token.Text.size() == 1);

    CHECK_FALSE(ctx.HasErrors());
  }
}

TEST_CASE("lexer: NextToken(BadToken)", "[lexer]") {
  auto const source = GENERATE(as<std::string>(), "@", "#", "%", "&", "~");
  auto ctx = ParseContext{};
  auto lexer = Lexer(ctx, source);

  auto token = lexer.NextToken();
  CHECK(token.Type == SyntaxTokenType::BadToken);
  CHECK(token.Position == 0);
  CHECK(token.Text.size() == 1);

  CHECK(ctx.HasErrors());
}