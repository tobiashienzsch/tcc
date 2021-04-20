#include "lexer.hpp"

#include "catch2/catch.hpp"

TEST_CASE("lexer: operator", "[lexer]") {
  auto const source = GENERATE(as<std::string>(), "+", "-", "*", "/");
  auto ctx = parse_context{};

  CHECK(lexer(ctx, "+").next_token().type == syntax_token_type::plus);
  CHECK(lexer(ctx, "-").next_token().type == syntax_token_type::minus);
  CHECK(lexer(ctx, "*").next_token().type == syntax_token_type::star);
  CHECK(lexer(ctx, "/").next_token().type == syntax_token_type::slash);
  CHECK(lexer(ctx, "(").next_token().type == syntax_token_type::open_brace);
  CHECK(lexer(ctx, ")").next_token().type == syntax_token_type::close_brace);

  CHECK_FALSE(ctx.has_errors());
}

TEST_CASE("lexer: NextToken", "[lexer]") {
  SECTION("No spaces") {
    auto const source = GENERATE(as<std::string>(), "42+2", "12+3", "99+0");
    auto ctx = parse_context{};
    auto lex = lexer(ctx, source);

    auto token = lex.next_token();
    CHECK(token.type == syntax_token_type::literal_integer);
    CHECK(token.position == 0);
    CHECK(token.text.size() == 2);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::plus);
    CHECK(token.position == 2);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::literal_integer);
    CHECK(token.position == 3);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::end_of_file);
    CHECK(token.position == 4);
    CHECK(token.text.size() == 1);

    CHECK_FALSE(ctx.has_errors());
  }

  SECTION("With spaces") {
    auto const source = GENERATE(as<std::string>(), " 42 + 2", " 12 + 3");
    auto ctx = parse_context{};
    auto lex = lexer(ctx, source);

    auto token = lex.next_token();
    CHECK(token.type == syntax_token_type::whitespace);
    CHECK(token.position == 0);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::literal_integer);
    CHECK(token.position == 1);
    CHECK(token.text.size() == 2);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::whitespace);
    CHECK(token.position == 3);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::plus);
    CHECK(token.position == 4);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::whitespace);
    CHECK(token.position == 5);
    CHECK(token.text.size() == 1);

    token = lex.next_token();
    CHECK(token.type == syntax_token_type::literal_integer);
    CHECK(token.position == 6);
    CHECK(token.text.size() == 1);

    CHECK_FALSE(ctx.has_errors());
  }
}

TEST_CASE("lexer: NextToken(bad_token)", "[lexer]") {
  auto const source = GENERATE(as<std::string>(), "@", "#", "%", "&", "~");
  auto ctx = parse_context{};
  auto lex = lexer(ctx, source);

  auto token = lex.next_token();
  CHECK(token.type == syntax_token_type::bad_token);
  CHECK(token.position == 0);
  CHECK(token.text.size() == 1);

  CHECK(ctx.has_errors());
}