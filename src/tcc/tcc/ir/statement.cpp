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
                       [&result](IRArgumentList const& args) {
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

    auto firstStr        = formatArgument(data.First);
    auto const secondStr = data.Second.has_value() ? formatArgument(data.Second.value()) : "";

    auto opCodeStr = std::stringstream {};
    opCodeStr << static_cast<tcc::IRByteCode>(data.Type);

    if (data.Type == IRByteCode::Call)
    {
        std::replace(std::begin(firstStr), std::end(firstStr), '%', '@');
    }

    return out << fmt::format("{0}\t:=\t{1}\t{2}\t{3}", data.Destination, opCodeStr.str(), firstStr,
                              secondStr);
}

auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream&
{
    return out << data.Name;
}

auto operator<<(std::ostream& out, IRPackage const& pkg) -> std::ostream&
{
    out << fmt::format("\n; {0}: functions={1}\n", pkg.Name, pkg.Functions.size());
    for (auto const& func : pkg.Functions)
    {
        out << fmt::format("; func {0}: args={1} locals={2} instructions={3}\n", func.Name,
                           func.Args.size(), func.Variables.size(), func.Blocks.size());

        for (auto const& block : func.Blocks)
        {
            out << fmt::format("; {0}.{1}:\n", func.Name[0], block.Name);
            for (auto const& x : block.Statements)
            {
                out << fmt::format("\t{}\n", x);
            }
            out << fmt::format("\n");
        }
    }

    return out;
}
}  // namespace tcc