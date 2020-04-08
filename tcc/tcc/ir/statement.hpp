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
struct IRStatement
{
    using Argument         = std::variant<std::uint32_t, std::string>;
    using OptionalArgument = std::optional<Argument>;

    tcc::IRByteCode type;
    std::string destination;
    Argument first;
    OptionalArgument second;
    bool isTemporary {true};
};

auto operator<<(std::ostream& out, IRStatement const& data) -> std::ostream&;

using IRStatementList = std::vector<IRStatement>;

struct IRFunction
{
    std::string name = "";
    std::vector<std::string> const args;
    std::map<std::string, int> variables = {};
    IRStatementList statements           = {};
};

struct IRPackage
{
    std::string name                  = "";
    std::vector<IRFunction> functions = {};
};

inline auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream& { return out << data.name; }
inline auto operator<<(std::ostream& out, IRPackage const& pkg) -> std::ostream&
{
    out << fmt::format("\n{0}: functions={1}\n", pkg.name, pkg.functions.size());
    for (auto const& func : pkg.functions)
    {
        out << fmt::format("\nfunc {0}: args={1} locals={2} instructions={3}\nentry:\n", func.name, func.args.size(),
                           func.variables.size(), func.statements.size());
        for (auto const& x : func.statements)
        {
            out << fmt::format("\t{}\n", x);
        }
    }

    return out;
}

}  // namespace tcc