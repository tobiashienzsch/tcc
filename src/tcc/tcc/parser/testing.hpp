#pragma once

#include <sstream>

namespace tcc::parser::testing
{

class NullBuffer final : public std::streambuf
{
public:
    auto overflow(int c) -> int override { return c; }
};

}  // namespace tcc::parser::testing