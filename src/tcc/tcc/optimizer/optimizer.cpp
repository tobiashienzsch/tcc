#include "tcc/optimizer/optimizer.hpp"

namespace tcc
{

auto Optimizer::Optimize() -> void
{
    auto const passes = 12U;
    for (auto x = 0U; x < passes; x++)
    {
        tcc::IgnoreUnused(x);

        for (auto& block : function_.Blocks)
        {
            std::for_each(std::begin(block.Statements), std::end(block.Statements),
                          [](auto& statement) { ReplaceWithConstantStore(statement); });

            std::for_each(
                std::begin(block.Statements), std::end(block.Statements),
                [&](auto& statement) { ReplaceVariableIfConstant(statement, block.Statements); });
        }
    }
    for (auto& block : function_.Blocks)
    {
        DeleteUnusedStatements(block.Statements);
    }
}

auto Optimizer::DeleteUnusedStatements(IRStatementList& statementList) -> bool
{
    while (true)
    {
        auto const elementToDelete
            = std::find_if(std::begin(statementList), std::end(statementList),
                           [&statementList](auto const& statement) {
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

auto Optimizer::IsUnusedStatement(IRStatement const& statement, IRStatementList const& statementList)
    -> bool
{
    return statement.IsTemporary
           && !std::any_of(std::begin(statementList), std::end(statementList),
                           [&statement](IRStatement const& item) {
                               auto result = false;

                               std::visit(tcc::overloaded {
                                              [](std::uint32_t /*unused*/) { ; },
                                              [](IRArgumentList const& /*unused*/) { ; },
                                              [&statement, &result](std::string const& name) {
                                                  if (name == statement.Destination)
                                                  {
                                                      result = true;
                                                  }
                                              },
                                          },
                                          item.First);

                               if (item.Second.has_value())
                               {
                                   std::visit(tcc::overloaded {
                                                  [](std::uint32_t /*unused*/) { ; },
                                                  [&statement, &result](IRArgumentList const& args) {
                                                      for (auto const& name : args)
                                                      {
                                                          if (name == statement.Destination)
                                                          {
                                                              result = true;
                                                          }
                                                      }
                                                  },
                                                  [&statement, &result](std::string const& name) {
                                                      if (name == statement.Destination)
                                                      {
                                                          result = true;
                                                      }
                                                  },
                                              },
                                              item.Second.value());
                               }

                               return result;
                           });
}

auto Optimizer::ReplaceVariableIfConstant(IRStatement& statement, IRStatementList& statementList)
    -> bool
{
    if (isConstantStoreExpression(statement))
    {
        for (auto& otherStatement : statementList)
        {
            if (otherStatement.Type != IRByteCode::Load)
            {
                // first
                std::visit(tcc::overloaded {
                               [](IRArgumentList const& /*unused*/) { ; },
                               [](std::uint32_t /*unused*/) { ; },
                               [&statement, &otherStatement](std::string const& name) {
                                   if (name == statement.Destination)
                                   {
                                       otherStatement.First
                                           = std::get<std::uint32_t>(statement.First);
                                   };
                               },
                           },
                           otherStatement.First);

                // second
                if (otherStatement.Second.has_value())
                {
                    std::visit(tcc::overloaded {
                                   [](IRArgumentList const& /*unused*/) { ; },
                                   [](std::uint32_t /*unused*/) { ; },
                                   [&statement, &otherStatement](std::string const& name) {
                                       if (name == statement.Destination)
                                       {
                                           otherStatement.Second
                                               = std::get<std::uint32_t>(statement.First);
                                       };
                                   },
                               },
                               otherStatement.Second.value());
                }
            }
        }

        return true;
    }

    return false;
}

// replace constant expression with store of result.
auto Optimizer::ReplaceWithConstantStore(IRStatement& statement) -> bool
{
    if (isConstantBinaryExpression(statement))
    {
        auto const first  = std::get<std::uint32_t>(statement.First);
        auto const second = std::get<std::uint32_t>(statement.Second.value());

        switch (statement.Type)
        {
            case IRByteCode::Addition: statement.First = first + second; break;
            case IRByteCode::Subtraction: statement.First = first - second; break;
            case IRByteCode::Multiplication: statement.First = first * second; break;
            case IRByteCode::Division: statement.First = first / second; break;
            default: break;
        }

        statement.Second = std::nullopt;
        statement.Type   = IRByteCode::Store;

        return true;
    }

    return false;
}

auto Optimizer::isConstantArgument(IRStatement::Argument const& argument) -> bool
{
    auto returnValue = bool {false};
    std::visit(tcc::overloaded {
                   [&returnValue](int /*unused*/) { returnValue = true; },
                   [&returnValue](IRArgumentList const& /*unused*/) { returnValue = false; },
                   [&returnValue](const std::string& /*unused*/) { returnValue = false; },
               },
               argument);

    return returnValue;
}

auto Optimizer::isConstantArgument(IRStatement::OptionalArgument const& argument) -> bool
{
    return argument.has_value() ? isConstantArgument(argument.value()) : false;
}

auto Optimizer::isConstantStoreExpression(IRStatement const& statement) -> bool
{
    return statement.Type == IRByteCode::Store && isConstantArgument(statement.First);
}

auto Optimizer::isConstantBinaryExpression(IRStatement const& statement) -> bool
{
    return isBinaryOperation(statement.Type) && isConstantArgument(statement.First)
           && isConstantArgument(statement.Second);
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