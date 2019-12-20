#pragma once

#include "tcc/core/core.hpp"

#include "vm.hpp"

#include <optional>
#include <ostream>
#include <variant>

namespace tcc
{
struct ThreeAddressCode
{
    tcc::byte_code type;
    std::string destination;
    std::variant<int, std::string> first;
    std::optional<std::variant<int, std::string>> second;
};

auto operator<<(std::ostream& out, ThreeAddressCode const& data) -> std::ostream&;

}  // namespace tcc