#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "tcc/ir/instruction_set.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
struct IRStatement
{
    using Argument         = std::variant<std::uint32_t, std::string, std::vector<std::string>>;
    using OptionalArgument = std::optional<Argument>;

    tcc::IRByteCode type;
    std::string destination;
    Argument first;
    OptionalArgument second;
    bool isTemporary {true};
};

auto operator<<(std::ostream& out, IRStatement const& data) -> std::ostream&;

using IRStatementList = std::vector<IRStatement>;

struct IRBasicBlock
{
    std::string name           = "";
    IRStatementList statements = {};
};

struct IRFunction
{
    std::string name                     = "";
    std::map<std::string, int> args      = {};
    std::map<std::string, int> variables = {};
    std::vector<IRBasicBlock> blocks     = {};
};

auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream&;

struct IRPackage
{
    std::string name                  = "";
    std::vector<IRFunction> functions = {};
};

auto operator<<(std::ostream& out, IRPackage const& pkg) -> std::ostream&;

}  // namespace tcc