#include "tcc/asm/assembly_generator.hpp"

#include "tsl/tsl.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <variant>

namespace tcc
{
auto AssemblyGenerator::Build(tcc::IRPackage const& package) -> std::vector<int64_t>
{
    auto result               = std::vector<int64_t> {};
    auto functionPlaceholders = std::map<int, std::string> {};
    auto functionPositions    = std::map<std::string, int> {};

    for (auto const& function : package.functions)
    {
        functionPositions.insert({function.name, result.size()});
        auto const& statements = function.statements;
        // auto const numLocals = function.variables.size();

        auto localVars = std::vector<std::string> {};
        for (auto const& var : function.variables)
        {
            localVars.push_back(var.first);
            result.push_back(tcc::ByteCode::ICONST);
            result.push_back(0);
        }

        for (IRStatement const& statement : statements)
        {
            auto const PushConstArgument = [&]() -> void {
                auto const& first = statement.first;
                if (auto* value = std::get_if<std::uint32_t>(&first); value != nullptr)
                {
                    result.push_back(tcc::ByteCode::ICONST);
                    result.push_back(*value);
                }

                if (statement.second.has_value())
                {
                    auto const& second = statement.second.value();
                    if (auto* value = std::get_if<std::uint32_t>(&second); value != nullptr)
                    {
                        result.push_back(tcc::ByteCode::ICONST);
                        result.push_back(*value);
                    }
                }
            };

            switch (statement.type)
            {
                case IRByteCode::Store:
                {
                    if (auto* value = std::get_if<std::uint32_t>(&statement.first); value != nullptr)
                    {
                        result.push_back(tcc::ByteCode::ICONST);
                        result.push_back(*value);
                    }
                    auto const destIter  = std::find(std::begin(localVars), std::end(localVars),
                                                    std::string(1, statement.destination[0]));
                    auto const destIndex = static_cast<int>(destIter - std::begin(localVars));

                    result.push_back(tcc::ByteCode::STORE);
                    result.push_back(destIndex);

                    break;
                }

                case IRByteCode::Load:
                {
                    auto const destIter  = std::find(std::begin(localVars), std::end(localVars),
                                                    std::string(1, std::get<std::string>(statement.first)[0]));
                    auto const destIndex = static_cast<int>(destIter - std::begin(localVars));

                    result.push_back(tcc::ByteCode::LOAD);
                    result.push_back(destIndex);

                    break;
                }

                case IRByteCode::Addition:
                {
                    PushConstArgument();
                    result.push_back(tcc::ByteCode::IADD);
                    break;
                }
                case IRByteCode::Subtraction:
                {
                    PushConstArgument();
                    result.push_back(tcc::ByteCode::ISUB);
                    break;
                }
                case IRByteCode::Multiplication:
                {
                    PushConstArgument();
                    result.push_back(tcc::ByteCode::IMUL);
                    break;
                }

                case IRByteCode::Call:
                {
                    result.push_back(tcc::ByteCode::CALL);
                    auto funcToCall = std::get<std::string>(statement.first);
                    functionPlaceholders.insert({result.size(), funcToCall});
                    result.push_back(9999);  // func addr
                    TCC_ASSERT(statement.second.has_value(), "Function call should have an arg list");
                    auto const numArgs = std::get<std::vector<std::string>>(statement.second.value()).size();
                    result.push_back(numArgs);  // func addr
                    break;
                }

                case IRByteCode::Return:
                {
                    PushConstArgument();
                    if (function.name == "main")
                    {
                        result.push_back(tcc::ByteCode::EXIT);
                        break;
                    }
                    result.push_back(tcc::ByteCode::RET);
                    break;
                }

                    // case IRByteCode::Division:
                    // {
                    //     result.push_back(tcc::ByteCode::IDIV);
                    //     fmt::print("DIV,\n");
                    //     break;
                    // }

                default: break;
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

        result.at(pos) = search->second;
    }
    return result;
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