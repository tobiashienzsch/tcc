#include "token.hpp"

auto operator<<(std::ostream &out, syntax_token_type const type)
    -> std::ostream & {
  switch (type) {
#define TCC_TOKEN_TYPE(x)                                                      \
  case syntax_token_type::x: {                                                 \
    out << #x;                                                                 \
    break;                                                                     \
  }
    TCC_TOKENS
#undef TCC_TOKEN_TYPE
  }

  return out;
}