#pragma once

#include <vector>

#include "tcc/ir/statement.hpp"

namespace tcc
{
using Assembly = std::pair<std::vector<int64_t>, int64_t>;
class AssemblyGenerator
{
public:
    static auto Build(tcc::IRPackage const& package) -> Assembly;
    static auto Print(std::vector<int64_t> const& assembly) -> void;
};
}  // namespace tcc