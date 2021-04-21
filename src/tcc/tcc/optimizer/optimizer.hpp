#pragma once

#include <algorithm>

#include "tcc/ir/statement.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{

class Optimizer
{
public:
    explicit Optimizer(IRFunction& scope) : function_(scope) { }

    auto optimize() -> void;

    static auto deleteUnusedStatements(IRStatementList& statementList) -> bool;
    static auto isUnusedStatement(IRStatement const& statement, IRStatementList const& statementList) -> bool;
    static auto replaceVariableIfConstant(IRStatement& statement, IRStatementList& statementList) -> bool;

    // replace constant expression with store of result.
    static auto replaceWithConstantStore(IRStatement& statement) -> bool;
    static auto isConstantArgument(IRStatement::Argument const& argument) -> bool;
    static auto isConstantArgument(IRStatement::OptionalArgument const& argument) -> bool;
    static auto isConstantStoreExpression(IRStatement const& statement) -> bool;
    static auto isConstantBinaryExpression(IRStatement const& statement) -> bool;
    static auto isBinaryOperation(IRByteCode op) noexcept -> bool;

private:
    IRFunction& function_;
};
}  // namespace tcc