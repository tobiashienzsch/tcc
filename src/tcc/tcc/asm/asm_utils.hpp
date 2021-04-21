#pragma once

#include "tcc/asm/asm.hpp"

#include <iosfwd>

namespace tcc
{
class ASMUtils
{
public:
    static auto prettyPrint(std::ostream& out, Assembly const& assembly) -> void;
};
}  // namespace tcc