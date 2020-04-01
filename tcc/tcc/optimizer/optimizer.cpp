#include "tcc/optimizer/optimizer.hpp"

namespace tcc {
auto Optimizer::Optimize() -> void {
  // fmt::print("Before: {} lines\n", m_mainScope.statements.size());
  // for (ThreeAddressCode const& x : m_mainScope.statements) std::cout << x <<
  // '\n';

  for (auto x = 0u; x < 7; x++) {
    tcc::IgnoreUnused(x);

    std::for_each(std::begin(m_mainScope.statements), std::end(m_mainScope.statements),
                  [](auto& statement) { ReplaceWithConstantStore(statement); });

    std::for_each(std::begin(m_mainScope.statements), std::end(m_mainScope.statements),
                  [&](auto& statement) { ReplaceVariableIfConstant(statement, m_mainScope.statements); });
  }

  DeleteUnusedStatements(m_mainScope.statements);

  fmt::print("\n\nAfter: {} lines\n", m_mainScope.statements.size());
  for (ThreeAddressCode const& x : m_mainScope.statements) std::cout << x << '\n';
}

auto Optimizer::DeleteUnusedStatements(StatementList& statementList) -> bool {
  while (true) {
    auto const elementToDelete =
        std::find_if(std::begin(statementList), std::end(statementList),
                     [&statementList](auto const& statement) { return IsUnusedStatement(statement, statementList); });

    if (elementToDelete == std::end(statementList)) return false;

    statementList.erase(elementToDelete);
  }

  return false;
}

auto Optimizer::IsUnusedStatement(ThreeAddressCode const& statement, StatementList const& statementList) -> bool {
  return statement.isTemporary &&
         !std::any_of(std::begin(statementList), std::end(statementList), [&statement](ThreeAddressCode const& item) {
           auto result = false;

           std::visit(tcc::overloaded{
                          [](int) { ; },
                          [&statement, &result](std::string const& name) {
                            if (name == statement.destination) result = true;
                          },
                      },
                      item.first);

           if (item.second.has_value()) {
             std::visit(tcc::overloaded{
                            [](int) { ; },
                            [&statement, &result](std::string const& name) {
                              if (name == statement.destination) result = true;
                            },
                        },
                        item.second.value());
           }

           return result;
         });
}

auto Optimizer::ReplaceVariableIfConstant(ThreeAddressCode& statement, StatementList& statementList) -> bool {
  if (isConstantStoreExpression(statement)) {
    for (auto& otherStatement : statementList) {
      if (otherStatement.type != byte_code::op_load) {
        // first
        std::visit(tcc::overloaded{
                       [](int) { ; },
                       [&statement, &otherStatement](std::string const& name) {
                         if (name == statement.destination) {
                           otherStatement.first = std::get<int>(statement.first);
                         };
                       },
                   },
                   otherStatement.first);

        // second
        if (otherStatement.second.has_value()) {
          std::visit(tcc::overloaded{
                         [](int) { ; },
                         [&statement, &otherStatement](std::string const& name) {
                           if (name == statement.destination) {
                             otherStatement.second = std::get<int>(statement.first);
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
auto Optimizer::ReplaceWithConstantStore(ThreeAddressCode& statement) -> bool {
  if (isConstantBinaryExpression(statement)) {
    auto const first = std::get<int>(statement.first);
    auto const second = std::get<int>(statement.second.value());

    switch (statement.type) {
      case byte_code::op_add:
        statement.first = first + second;
        break;
      case byte_code::op_sub:
        statement.first = first - second;
        break;
      case byte_code::op_mul:
        statement.first = first * second;
        break;
      case byte_code::op_div:
        statement.first = first / second;
        break;
      default:
        break;
    }

    statement.second = std::nullopt;
    statement.type = byte_code::op_store;

    return true;
  }

  return false;
}

auto Optimizer::isConstantArgument(ThreeAddressCode::Argument const& argument) -> bool {
  auto returnValue = bool{false};
  std::visit(tcc::overloaded{
                 [&returnValue](int) { returnValue = true; },
                 [&returnValue](const std::string&) { returnValue = false; },
             },
             argument);

  return returnValue;
}

auto Optimizer::isConstantArgument(ThreeAddressCode::OptionalArgument const& argument) -> bool {
  if (argument.has_value()) {
    return isConstantArgument(argument.value());
  }

  return false;
}

auto Optimizer::isConstantStoreExpression(ThreeAddressCode const& statement) -> bool {
  if (statement.type == byte_code::op_store && isConstantArgument(statement.first)) {
    return true;
  }
  return false;
}

auto Optimizer::isConstantBinaryExpression(ThreeAddressCode const& statement) -> bool {
  if (isBinaryOperation(statement.type)) {
    if (isConstantArgument(statement.first) && isConstantArgument(statement.second)) {
      return true;
    }
  }

  return false;
}

auto Optimizer::isBinaryOperation(byte_code op) noexcept -> bool {
  if (op == byte_code::op_add) return true;
  if (op == byte_code::op_sub) return true;
  if (op == byte_code::op_mul) return true;
  if (op == byte_code::op_div) return true;
  return false;
}
}  // namespace tcc