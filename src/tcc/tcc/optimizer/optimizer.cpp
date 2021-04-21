#include "tcc/optimizer/optimizer.hpp"

namespace tcc
{

auto Optimizer::optimize() -> void
{
    auto const passes = 12U;
    for (auto x = 0U; x < passes; x++)
    {
        tcc::ignoreUnused(x);

        for (auto& block : function_.blocks)
        {
            std::for_each(std::begin(block.statements), std::end(block.statements),
                          [](auto& statement) { replaceWithConstantStore(statement); });

            std::for_each(std::begin(block.statements), std::end(block.statements),
                          [&](auto& statement) { replaceVariableIfConstant(statement, block.statements); });
        }
    }
    for (auto& block : function_.blocks) { deleteUnusedStatements(block.statements); }
}

auto Optimizer::deleteUnusedStatements(IRStatementList& statementList) -> bool
{
    while (true)
    {
        auto const elementToDelete
            = std::find_if(std::begin(statementList), std::end(statementList), [&statementList](auto const& statement) {
                  return isUnusedStatement(statement, statementList);
              });

        if (elementToDelete == std::end(statementList)) { return false; }

        statementList.erase(elementToDelete);
    }

    return false;
}

auto Optimizer::isUnusedStatement(IRStatement const& statement, IRStatementList const& statementList) -> bool
{
    return statement.isTemporary
           && !std::any_of(std::begin(statementList), std::end(statementList), [&statement](IRStatement const& item) {
                  auto result = false;

                  std::visit(tcc::Overloaded {
                                 [](std::uint32_t /*unused*/) { ; },
                                 [](IRArgumentList const& /*unused*/) { ; },
                                 [&statement, &result](std::string const& name) {
                                     if (name == statement.destination) { result = true; }
                                 },
                             },
                             item.first);

                  if (item.second.has_value())
                  {
                      std::visit(tcc::Overloaded {
                                     [](std::uint32_t /*unused*/) { ; },
                                     [&statement, &result](IRArgumentList const& args) {
                                         for (auto const& name : args)
                                         {
                                             if (name == statement.destination) { result = true; }
                                         }
                                     },
                                     [&statement, &result](std::string const& name) {
                                         if (name == statement.destination) { result = true; }
                                     },
                                 },
                                 item.second.value());
                  }

                  return result;
              });
}

auto Optimizer::replaceVariableIfConstant(IRStatement& statement, IRStatementList& statementList) -> bool
{
    if (isConstantStoreExpression(statement))
    {
        for (auto& otherStatement : statementList)
        {
            if (otherStatement.type != IRByteCode::Load)
            {
                // first
                std::visit(tcc::Overloaded {
                               [](IRArgumentList const& /*unused*/) { ; },
                               [](std::uint32_t /*unused*/) { ; },
                               [&statement, &otherStatement](std::string const& name) {
                                   if (name == statement.destination)
                                   { otherStatement.first = std::get<std::uint32_t>(statement.first); };
                               },
                           },
                           otherStatement.first);

                // second
                if (otherStatement.second.has_value())
                {
                    std::visit(tcc::Overloaded {
                                   [](IRArgumentList const& /*unused*/) { ; },
                                   [](std::uint32_t /*unused*/) { ; },
                                   [&statement, &otherStatement](std::string const& name) {
                                       if (name == statement.destination)
                                       { otherStatement.second = std::get<std::uint32_t>(statement.first); };
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
auto Optimizer::replaceWithConstantStore(IRStatement& statement) -> bool
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

auto Optimizer::isConstantArgument(IRStatement::Argument const& argument) -> bool
{
    auto returnValue = bool {false};
    std::visit(tcc::Overloaded {
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
    return statement.type == IRByteCode::Store && isConstantArgument(statement.first);
}

auto Optimizer::isConstantBinaryExpression(IRStatement const& statement) -> bool
{
    return isBinaryOperation(statement.type) && isConstantArgument(statement.first)
           && isConstantArgument(statement.second);
}

auto Optimizer::isBinaryOperation(IRByteCode op) noexcept -> bool
{
    if (op == IRByteCode::Addition) { return true; }
    if (op == IRByteCode::Subtraction) { return true; }
    if (op == IRByteCode::Multiplication) { return true; }
    if (op == IRByteCode::Division) { return true; }
    return false;
}
}  // namespace tcc