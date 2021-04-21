#pragma once

#include "tcc/asm/asm.hpp"
#include "tcc/ir/statement.hpp"

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto build(tcc::IRPackage const& package) -> Assembly;
};
}  // namespace tcc