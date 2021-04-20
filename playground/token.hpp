#ifndef TCC_PLAYGROUND_TOKEN_HPP
#define TCC_PLAYGROUND_TOKEN_HPP

#include <cstddef>
#include <ostream>
#include <string>

// clang-format off
#define TCC_TOKENS                  \
    TCC_TOKEN_TYPE(bad_token)        \
    TCC_TOKEN_TYPE(whitespace)      \
    TCC_TOKEN_TYPE(literal_integer)  \
    TCC_TOKEN_TYPE(identifier)      \
    TCC_TOKEN_TYPE(plus)            \
    TCC_TOKEN_TYPE(minus)           \
    TCC_TOKEN_TYPE(star)            \
    TCC_TOKEN_TYPE(slash)           \
    TCC_TOKEN_TYPE(open_brace)       \
    TCC_TOKEN_TYPE(close_brace)      \
    TCC_TOKEN_TYPE(end_of_file)
// clang-format on

#define TCC_TOKEN_TYPE(x) x,
enum class syntax_token_type { TCC_TOKENS };
#undef TCC_TOKEN_TYPE

auto operator<<(std::ostream &out, syntax_token_type type) -> std::ostream &;

struct syntax_token {
  syntax_token_type type;
  std::size_t position;
  std::string text;
};

[[nodiscard]] inline constexpr auto
get_binary_opprecedence(syntax_token_type type) noexcept -> int {
  switch (type) {
  case syntax_token_type::star:
  case syntax_token_type::slash: {
    return 2;
  }

  case syntax_token_type::plus:
  case syntax_token_type::minus: {
    return 1;
  }

  default: {
    return 0;
  }
  }
}

[[nodiscard]] inline constexpr auto
get_unary_opprecedence(syntax_token_type type) noexcept -> int {
  switch (type) {
  case syntax_token_type::plus:
  case syntax_token_type::minus: {
    return 1;
  }

  default: {
    return 0;
  }
  }
}

#endif // TCC_PLAYGROUND_TOKEN_HPP
