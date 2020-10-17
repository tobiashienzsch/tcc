#include "tcc/asm/assembly_generator.hpp"

#include "tcsl/tcsl.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <variant>

class FunctionPosition
{
public:
    constexpr explicit FunctionPosition(std::size_t pos) : pos_ {pos} { }
    [[nodiscard]] constexpr auto Value() const noexcept { return pos_; }

    friend auto operator<(FunctionPosition const& lhs, FunctionPosition const& rhs)
    {
        return lhs.Value() < rhs.Value();
    }

private:
    std::size_t pos_;
};

namespace tcc
{
auto AssemblyGenerator::Build(tcc::IRPackage const& package) -> Assembly
{
    auto assembly             = std::vector<int64_t> {};
    auto mainPosition         = std::optional<FunctionPosition> {std::nullopt};
    auto functionPlaceholders = std::map<FunctionPosition, std::string> {};
    auto functionPositions    = std::map<std::string, FunctionPosition> {};

    for (auto const& function : package.Functions)
    {
        auto funcPos = FunctionPosition {assembly.size()};
        if (function.Name == "main")
        {
            mainPosition = funcPos;
        }

        functionPositions.insert({function.Name, funcPos});

        auto argVars = IRArgumentList {};
        for (auto const& arg : function.Args)
        {
            argVars.push_back(arg.first);
        }

        auto locals = IRArgumentList {};
        for (auto const& var : function.Variables)
        {
            locals.push_back(var.first);
            assembly.push_back(tcc::ByteCode::ICONST);
            assembly.push_back(0);
        }

        for (auto const& block : function.Blocks)
        {
            auto const& statements = block.Statements;
            for (IRStatement const& statement : statements)
            {
                auto const PushConstArgument = [&]() -> void {
                    auto const& first = statement.First;
                    if (auto const* value = std::get_if<std::uint32_t>(&first); value != nullptr)
                    {
                        assembly.push_back(tcc::ByteCode::ICONST);
                        assembly.push_back(*value);
                    }

                    if (statement.Second.has_value())
                    {
                        auto const& second = statement.Second.value();
                        if (auto const* value = std::get_if<std::uint32_t>(&second); value != nullptr)
                        {
                            assembly.push_back(tcc::ByteCode::ICONST);
                            assembly.push_back(*value);
                        }
                    }
                };

                switch (statement.Type)
                {
                    case IRByteCode::Store:
                    {
                        if (auto const* value = std::get_if<std::uint32_t>(&statement.First);
                            value != nullptr)
                        {
                            assembly.push_back(tcc::ByteCode::ICONST);
                            assembly.push_back(*value);
                        }
                        auto const iter  = std::find(std::begin(locals), std::end(locals),
                                                    std::string(1, statement.Destination[0]));
                        auto const index = static_cast<int>(iter - std::begin(locals));

                        assembly.push_back(tcc::ByteCode::STORE);
                        assembly.push_back(index + 1);

                        break;
                    }

                    case IRByteCode::Load:
                    {
                        auto const inLocalVars
                            = std::find(std::begin(locals), std::end(locals),
                                        std::string(1, std::get<std::string>(statement.First)[0]));
                        if (inLocalVars != std::end(locals))
                        {
                            auto const index = static_cast<int>(inLocalVars - std::begin(locals));
                            assembly.push_back(tcc::ByteCode::LOAD);
                            assembly.push_back(index + 1);
                            break;
                        }

                        auto const inArgs
                            = std::find(std::begin(argVars), std::end(argVars),
                                        std::string(1, std::get<std::string>(statement.First)[0]));
                        if (inArgs != std::end(argVars))
                        {
                            // offset to function arguments on the stack is -3.
                            // Since we calculate the index with std::end, which
                            // is one past the last element we subtract from -2.
                            auto const index = -2 - static_cast<int>(std::end(argVars) - inArgs);
                            assembly.push_back(tcc::ByteCode::LOAD);
                            assembly.push_back(index);
                            break;
                        }

                        TCC_ASSERT(false, "");
                        break;
                    }

                    case IRByteCode::Addition:
                    {
                        PushConstArgument();
                        assembly.push_back(tcc::ByteCode::IADD);
                        break;
                    }
                    case IRByteCode::Subtraction:
                    {
                        PushConstArgument();
                        assembly.push_back(tcc::ByteCode::ISUB);
                        break;
                    }
                    case IRByteCode::Multiplication:
                    {
                        PushConstArgument();
                        assembly.push_back(tcc::ByteCode::IMUL);
                        break;
                    }

                    case IRByteCode::Call:
                    {
                        assembly.push_back(tcc::ByteCode::CALL);
                        auto funcToCall = std::get<std::string>(statement.First);
                        functionPlaceholders.insert({FunctionPosition {assembly.size()}, funcToCall});
                        assembly.push_back(9999);  // func addr
                        TCC_ASSERT(statement.Second.has_value(),
                                   "Function call should have an arg list");
                        auto const numArgs
                            = std::get<IRArgumentList>(statement.Second.value()).size();
                        assembly.push_back(numArgs);  // func addr
                        break;
                    }

                    case IRByteCode::Return:
                    {
                        PushConstArgument();
                        assembly.push_back(tcc::ByteCode::RET);
                        break;
                    }

                        // case IRByteCode::Division:
                        // {
                        //     assembly.push_back(tcc::ByteCode::IDIV);
                        //     fmt::print("DIV,\n");
                        //     break;
                        // }

                    default: break;
                }
            }
        }
    }

    for (auto const& placeholder : functionPlaceholders)
    {
        auto const pos        = placeholder.first;
        auto const funcToCall = placeholder.second;

        auto search = functionPositions.find(funcToCall);
        if (search == functionPositions.end())
        {
            TCC_ASSERT(false, "Function not found");
        }

        assembly.at(pos.Value()) = search->second.Value();
    }

    // Append __init function. This calls main.
    TCC_ASSERT(mainPosition.has_value(), "");
    auto const entryPoint = assembly.size();
    assembly.push_back(ByteCode::CALL);
    assembly.push_back(mainPosition.value().Value());
    assembly.push_back(0);
    assembly.push_back(ByteCode::EXIT);

    return Assembly {assembly, entryPoint};
}

auto AssemblyGenerator::Print(std::vector<int64_t> const& assembly) -> void
{
    auto ops = std::set<tcc::ByteCode> {};
    auto out = std::string {};

    for (auto i = 0UL; i < assembly.size();)
    {
        auto const op = static_cast<tcc::ByteCode>(assembly.at(i));
        ops.emplace(op);
        out.append(fmt::format("\t{0}", op));

        switch (op)
        {
            case ByteCode::RET: break;
            case ByteCode::EXIT: break;
            case ByteCode::IADD: break;
            case ByteCode::IMUL: break;
            case ByteCode::ISUB: break;
            case ByteCode::LOAD:
            {
                out.append(fmt::format(",\t{}", assembly.at(++i)));
                break;
            }
            case ByteCode::STORE:
            {
                out.append(fmt::format(",\t{}", assembly.at(++i)));
                break;
            }
            case ByteCode::ICONST:
            {
                out.append(fmt::format(",\t{}", assembly.at(++i)));
                break;
            }
            case ByteCode::CALL:
            {
                out.append(fmt::format(",\t{}", assembly.at(++i)));
                out.append(fmt::format(",\t{}", assembly.at(++i)));
                break;
            }
            default:
            {
                TCC_ASSERT(false, fmt::format("Unknown operation {}", op));
                std::exit(EXIT_FAILURE);
            }
        }

        i++;
        out.append(fmt::format("\n"));
    }

    fmt::print("\n// asm: length={0} ops={1}/{2}\n{3}\n", assembly.size(), ops.size(),
               static_cast<int>(ByteCode::NUM_OPCODES), out);
}

}  // namespace tcc