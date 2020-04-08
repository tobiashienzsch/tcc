#include "tcc/optimizer/optimizer.hpp"

namespace tcc
{

auto Optimizer::Optimize() -> void
{
    auto const passes = 12U;
    for (auto x = 0U; x < passes; x++)
    {
        tcc::IgnoreUnused(x);

        std::for_each(std::begin(m_mainScope.statements), std::end(m_mainScope.statements),
                      [](auto& statement) { ReplaceWithConstantStore(statement); });

        std::for_each(std::begin(m_mainScope.statements), std::end(m_mainScope.statements),
                      [&](auto& statement) { ReplaceVariableIfConstant(statement, m_mainScope.statements); });
    }

    DeleteUnusedStatements(m_mainScope.statements);
}

auto Optimizer::DeleteUnusedStatements(IRStatementList& statementList) -> bool
{
    while (true)
    {
        auto const elementToDelete
            = std::find_if(std::begin(statementList), std::end(statementList), [&statementList](auto const& statement) {
                  return IsUnusedStatement(statement, statementList);
              });

        if (elementToDelete == std::end(statementList))
        {
            return false;
        }

        statementList.erase(elementToDelete);
    }

    return false;
}

auto Optimizer::IsUnusedStatement(ThreeAddressCode const& statement, IRStatementList const& statementList) -> bool
{
    return statement.isTemporary
           && !std::any_of(std::begin(statementList), std::end(statementList),
                           [&statement](ThreeAddressCode const& item) {
                               auto result = false;

                               std::visit(tcc::overloaded {
                                              [](std::uint32_t /*unused*/) { ; },
                                              [&statement, &result](std::string const& name) {
                                                  if (name == statement.destination)
                                                  {
                                                      result = true;
                                                  }
                                              },
                                          },
                                          item.first);

                               if (item.second.has_value())
                               {
                                   std::visit(tcc::overloaded {
                                                  [](std::uint32_t /*unused*/) { ; },
                                                  [&statement, &result](std::string const& name) {
                                                      if (name == statement.destination)
                                                      {
                                                          result = true;
                                                      }
                                                  },
                                              },
                                              item.second.value());
                               }

                               return result;
                           });
}

auto Optimizer::ReplaceVariableIfConstant(ThreeAddressCode& statement, IRStatementList& statementList) -> bool
{
    if (isConstantStoreExpression(statement))
    {
        for (auto& otherStatement : statementList)
        {
            if (otherStatement.type != IRByteCode::Load)
            {
                // first
                std::visit(tcc::overloaded {
                               [](std::uint32_t /*unused*/) { ; },
                               [&statement, &otherStatement](std::string const& name) {
                                   if (name == statement.destination)
                                   {
                                       otherStatement.first = std::get<std::uint32_t>(statement.first);
                                   };
                               },
                           },
                           otherStatement.first);

                // second
                if (otherStatement.second.has_value())
                {
                    std::visit(tcc::overloaded {
                                   [](std::uint32_t /*unused*/) { ; },
                                   [&statement, &otherStatement](std::string const& name) {
                                       if (name == statement.destination)
                                       {
                                           otherStatement.second = std::get<std::uint32_t>(statement.first);
                                       };
                                   },
                               },
                               otherStatement.second.value());
                }
            }
        }

        return true;
    }

    return false;
}

// replace constant expression with store of result.
auto Optimizer::ReplaceWithConstantStore(ThreeAddressCode& statement) -> bool
{
    if (isConstantBinaryExpression(statement))
    {
        auto const first  = std::get<std::uint32_t>(statement.first);
        auto const second = std::get<std::uint32_t>(statement.second.value());

        switch (statement.type)
        {
            case IRByteCode::Addition: statement.first = first + second; break;
            case IRByteCode::Subtraction: statement.first = first - second; break;
            case IRByteCode::Multiplication: statement.first = first * second; break;
            case IRByteCode::Division: statement.first = first / second; break;
            default: break;
        }

        statement.second = std::nullopt;
        statement.type   = IRByteCode::Store;

        return true;
    }

    return false;
}

auto Optimizer::isConstantArgument(ThreeAddressCode::Argument const& argument) -> bool
{
    auto returnValue = bool {false};
    std::visit(tcc::overloaded {
                   [&returnValue](int /*unused*/) { returnValue = true; },
                   [&returnValue](const std::string& /*unused*/) { returnValue = false; },
               },
               argument);

    return returnValue;
}

auto Optimizer::isConstantArgument(ThreeAddressCode::OptionalArgument const& argument) -> bool
{
    if (argument.has_value())
    {
        return isConstantArgument(argument.value());
    }

    return false;
}

auto Optimizer::isConstantStoreExpression(ThreeAddressCode const& statement) -> bool
{
    return statement.type == IRByteCode::Store && isConstantArgument(statement.first);
}

auto Optimizer::isConstantBinaryExpression(ThreeAddressCode const& statement) -> bool
{
    if (isBinaryOperation(statement.type))
    {
        if (isConstantArgument(statement.first) && isConstantArgument(statement.second))
        {
            return true;
        }
    }

    return false;
}

auto Optimizer::isBinaryOperation(IRByteCode op) noexcept -> bool
{
    if (op == IRByteCode::Addition)
    {
        return true;
    }
    if (op == IRByteCode::Subtraction)
    {
        return true;
    }
    if (op == IRByteCode::Multiplication)
    {
        return true;
    }
    if (op == IRByteCode::Division)
    {
        return true;
    }
    return false;
}
}  // namespace tcc