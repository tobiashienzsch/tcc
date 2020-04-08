#pragma once

#include <sstream>

namespace tcc::parser::testing
{

class NullBuffer final : public std::streambuf
{
public:
    int overflow(int c) override { return c; }
};

}  // namespace tcc::parser::testing