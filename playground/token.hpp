#ifndef TCC_PLAYGROUND_TOKEN_HPP
#define TCC_PLAYGROUND_TOKEN_HPP

#include <cstddef>
#include <ostream>
#include <string>

// clang-format off
#define TCC_TOKENS                  \
    TCC_TOKEN_TYPE(BadToken)        \
    TCC_TOKEN_TYPE(Whitespace)      \
    TCC_TOKEN_TYPE(LiteralInteger)  \
    TCC_TOKEN_TYPE(Identifier)      \
    TCC_TOKEN_TYPE(Plus)            \
    TCC_TOKEN_TYPE(Minus)           \
    TCC_TOKEN_TYPE(Star)            \
    TCC_TOKEN_TYPE(Slash)           \
    TCC_TOKEN_TYPE(OpenBrace)       \
    TCC_TOKEN_TYPE(CloseBrace)      \
    TCC_TOKEN_TYPE(EndOfFile)
// clang-format on

#define TCC_TOKEN_TYPE(x) x,
enum class SyntaxTokenType { TCC_TOKENS };
#undef TCC_TOKEN_TYPE

auto operator<<(std::ostream &out, SyntaxTokenType type) -> std::ostream &;

struct SyntaxToken {
  SyntaxTokenType Type;
  std::size_t Position;
  std::string Text;
};

#endif // TCC_PLAYGROUND_TOKEN_HPP
