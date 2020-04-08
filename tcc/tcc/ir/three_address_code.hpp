#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "tcc/ir/instruction_set.hpp"
#include "tsl/tsl.hpp"

namespace tcc
{
struct ThreeAddressCode
{
    using Argument         = std::variant<std::uint32_t, std::string>;
    using OptionalArgument = std::optional<Argument>;

    tcc::IRByteCode type;
    std::string destination;
    Argument first;
    OptionalArgument second;
    bool isTemporary {true};
};

auto operator<<(std::ostream& out, ThreeAddressCode const& data) -> std::ostream&;

using StatementList = std::vector<ThreeAddressCode>;

struct IRFunction
{
    std::string name                     = "";
    StatementList statements             = {};
    std::map<std::string, int> variables = {};
};

inline auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream& { return out << data.name; }

}  // namespace tcc