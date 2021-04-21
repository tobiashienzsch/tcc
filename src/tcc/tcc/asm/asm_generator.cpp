#include "tcc/asm/asm_generator.hpp"

#include "tcsl/tcsl.hpp"

#include <algorithm>
#include <map>
#include <variant>

class FunctionPosition
{
public:
    constexpr explicit FunctionPosition(std::size_t pos) : pos_ {pos} { }
    [[nodiscard]] constexpr auto value() const noexcept { return pos_; }

    friend auto operator<(FunctionPosition const& lhs, FunctionPosition const& rhs)
    {
        return lhs.value() < rhs.value();
    }

private:
    std::size_t pos_;
};

namespace tcc
{
auto AssemblyGenerator::build(tcc::IRPackage const& package) -> Assembly
{
    auto assembly             = std::vector<int64_t> {};
    auto mainPosition         = std::optional<FunctionPosition> {std::nullopt};
    auto functionPlaceholders = std::map<FunctionPosition, std::string> {};
    auto functionPositions    = std::map<std::string, FunctionPosition> {};

    for (auto const& function : package.functions)
    {
        auto funcPos = FunctionPosition {assembly.size()};
        if (function.name == "main") { mainPosition = funcPos; }

        functionPositions.insert({function.name, funcPos});

        auto argVars = IRArgumentList {};
        for (auto const& arg : function.args) { argVars.pushBack(arg.first); }

        auto locals = IRArgumentList {};
        for (auto const& var : function.variables)
        {
            locals.pushBack(var.first);
            assembly.push_back(tcc::ByteCode::ICONST);
            assembly.push_back(0);
        }

        for (auto const& block : function.blocks)
        {
            auto const& statements = block.statements;
            for (IRStatement const& statement : statements)
            {
                auto const pushConstArgument = [&]() -> void {
                    auto const& first = statement.first;
                    if (auto const* value = std::get_if<std::uint32_t>(&first); value != nullptr)
                    {
                        assembly.push_back(tcc::ByteCode::ICONST);
                        assembly.push_back(*value);
                    }

                    if (statement.second.has_value())
                    {
                        auto const& second = statement.second.value();
                        if (auto const* value = std::get_if<std::uint32_t>(&second); value != nullptr)
                        {
                            assembly.push_back(tcc::ByteCode::ICONST);
                            assembly.push_back(*value);
                        }
                    }
                };

                switch (statement.type)
                {
                    case IRByteCode::Store:
                    {
                        if (auto const* value = std::get_if<std::uint32_t>(&statement.first); value != nullptr)
                        {
                            assembly.push_back(tcc::ByteCode::ICONST);
                            assembly.push_back(*value);
                        }
                        auto const iter
                            = std::find(std::begin(locals), std::end(locals), std::string(1, statement.destination[0]));
                        auto const index = static_cast<int>(iter - std::begin(locals));

                        assembly.push_back(tcc::ByteCode::STORE);
                        assembly.push_back(index + 1);

                        break;
                    }

                    case IRByteCode::Load:
                    {
                        auto const inLocalVars = std::find(std::begin(locals), std::end(locals),
                                                           std::string(1, std::get<std::string>(statement.first)[0]));
                        if (inLocalVars != std::end(locals))
                        {
                            auto const index = static_cast<int>(inLocalVars - std::begin(locals));
                            assembly.push_back(tcc::ByteCode::LOAD);
                            assembly.push_back(index + 1);
                            break;
                        }

                        auto const inArgs = std::find(std::begin(argVars), std::end(argVars),
                                                      std::string(1, std::get<std::string>(statement.first)[0]));
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
                        pushConstArgument();
                        assembly.push_back(tcc::ByteCode::IADD);
                        break;
                    }
                    case IRByteCode::Subtraction:
                    {
                        pushConstArgument();
                        assembly.push_back(tcc::ByteCode::ISUB);
                        break;
                    }
                    case IRByteCode::Multiplication:
                    {
                        pushConstArgument();
                        assembly.push_back(tcc::ByteCode::IMUL);
                        break;
                    }

                    case IRByteCode::Call:
                    {
                        assembly.push_back(tcc::ByteCode::CALL);
                        auto funcToCall = std::get<std::string>(statement.first);
                        functionPlaceholders.insert({FunctionPosition {assembly.size()}, funcToCall});
                        assembly.push_back(9999);  // func addr
                        TCC_ASSERT(statement.second.has_value(), "Function call should have an arg list");
                        auto const numArgs = std::get<IRArgumentList>(statement.second.value()).size();
                        assembly.push_back(numArgs);  // func addr
                        break;
                    }

                    case IRByteCode::Return:
                    {
                        pushConstArgument();
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
        if (search == functionPositions.end()) { TCC_ASSERT(false, "Function not found"); }

        assembly.at(pos.value()) = search->second.value();
    }

    // Append __init function. This calls main.
    TCC_ASSERT(mainPosition.has_value(), "");
    auto const entryPoint = assembly.size();
    assembly.push_back(ByteCode::CALL);
    assembly.push_back(mainPosition.value().value());
    assembly.push_back(0);
    assembly.push_back(ByteCode::EXIT);

    return Assembly {assembly, entryPoint};
}

}  // namespace tcc