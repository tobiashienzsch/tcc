#pragma once

#include <vector>

#include "tcc/ir/statement.hpp"

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(IRFunction const& function) -> std::vector<int64_t>;
};
}  // namespace tcc