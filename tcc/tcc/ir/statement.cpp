#include "tcc/ir/statement.hpp"

#include <sstream>

namespace tcc
{
auto operator<<(std::ostream& out, IRStatement const& data) -> std::ostream&
{
    auto const formatArgument = [](auto const& argument) {
        auto result = std::string {};
        std::visit(tcc::overloaded {
                       [&result](int const arg) { result = fmt::format("{}", arg); },
                       [&result](std::string const& arg) { result = fmt::format("%{}", arg); },
                       [&result](std::vector<std::string> const& args) {
                           result.append(fmt::format("[ "));
                           for (auto const& arg : args)
                           {
                               result.append(fmt::format("%{} ", arg));
                           }
                           result.append(fmt::format("]"));
                       },
                   },
                   argument);
        return result;
    };

    auto const firstStr  = formatArgument(data.first);
    auto const secondStr = data.second.has_value() ? formatArgument(data.second.value()) : "";

    auto opCodeStr = std::stringstream {};
    opCodeStr << static_cast<tcc::IRByteCode>(data.type);

    return out << fmt::format("{0}\t:=\t{1}\t{2}\t{3}", data.destination, opCodeStr.str(), firstStr, secondStr);
}

auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream& { return out << data.name; }
auto operator<<(std::ostream& out, IRPackage const& pkg) -> std::ostream&
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