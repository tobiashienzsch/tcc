#pragma once

#include <algorithm>

#include "tcc/ir/statement.hpp"
#include "tsl/tsl.hpp"

namespace tcc
{

class Optimizer
{
public:
    Optimizer(IRFunction& scope) : m_mainScope(scope) { }

    auto Optimize() -> void;

    static auto DeleteUnusedStatements(IRStatementList& statementList) -> bool;
    static auto IsUnusedStatement(IRStatement const& statement, IRStatementList const& statementList) -> bool;
    static auto ReplaceVariableIfConstant(IRStatement& statement, IRStatementList& statementList) -> bool;

    // replace constant expression with store of result.
    static auto ReplaceWithConstantStore(IRStatement& statement) -> bool;
    static auto isConstantArgument(IRStatement::Argument const& argument) -> bool;
    static auto isConstantArgument(IRStatement::OptionalArgument const& argument) -> bool;
    static auto isConstantStoreExpression(IRStatement const& statement) -> bool;
    static auto isConstantBinaryExpression(IRStatement const& statement) -> bool;
    static auto isBinaryOperation(IRByteCode op) noexcept -> bool;

private:
    IRFunction& m_mainScope;
};
}  // namespace tcc