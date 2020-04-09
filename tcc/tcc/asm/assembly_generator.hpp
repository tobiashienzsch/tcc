#pragma once

#include <vector>

#include "tcc/ir/statement.hpp"

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(tcc::IRPackage const& package) -> std::vector<int64_t>;
    static auto Print(std::vector<int64_t> const& assembly) -> void;
};
}  // namespace tcc