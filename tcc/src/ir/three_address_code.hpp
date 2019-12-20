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
    using Argument         = std::variant<int, std::string>;
    using OptionalArgument = std::optional<Argument>;

    tcc::byte_code type;
    std::string destination;
    Argument first;
    OptionalArgument second;
    bool isTemporary {true};
};

auto operator<<(std::ostream& out, ThreeAddressCode const& data) -> std::ostream&;

}  // namespace tcc