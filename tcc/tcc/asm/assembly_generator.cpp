#include "tcc/asm/assembly_generator.hpp"

#include <algorithm>
#include <variant>

#include "tsl/tsl.hpp"

namespace tcc
{
auto AssemblyGenerator::Build(tcc::IRFunction const& function) -> std::vector<int64_t>
{
    auto result = std::vector<int64_t> {};

    auto const& statements = function.statements;
    // auto const numLocals = function.variables.size();

    auto localVars = std::vector<std::string> {};
    for (auto const& var : function.variables)
    {
        localVars.push_back(var.first);
        result.push_back(tcc::ByteCode::ICONST);
        result.push_back(0);
    }

    for (ThreeAddressCode const& statement : statements)
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
                auto const destIter
                    = std::find(std::begin(localVars), std::end(localVars), std::string(1, statement.destination[0]));
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
            case IRByteCode::Return:
            {
                PushConstArgument();
                result.push_back(tcc::ByteCode::EXIT);
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

    result.push_back(tcc::ByteCode::EXIT);
    return result;
}
}  // namespace tcc