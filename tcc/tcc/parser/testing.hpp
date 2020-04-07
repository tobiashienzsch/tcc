#pragma once

#include <sstream>

namespace tcc::parser::testing {

class NullBuffer : public std::streambuf {
 public:
  int overflow(int c) { return c; }
};

}  // namespace tcc::parser::testing