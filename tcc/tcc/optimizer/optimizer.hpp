#pragma once

#include <algorithm>

#include "tcc/ir/three_address_code.hpp"
#include "tsl/tsl.hpp"

namespace tcc
{

class Optimizer
{
public:
    Optimizer(IRFunction& scope) : m_mainScope(scope) { }

    auto Optimize() -> void;

    static auto DeleteUnusedStatements(IRStatementList& statementList) -> bool;
    static auto IsUnusedStatement(ThreeAddressCode const& statement, IRStatementList const& statementList) -> bool;
    static auto ReplaceVariableIfConstant(ThreeAddressCode& statement, IRStatementList& statementList) -> bool;

    // replace constant expression with store of result.
    static auto ReplaceWithConstantStore(ThreeAddressCode& statement) -> bool;
    static auto isConstantArgument(ThreeAddressCode::Argument const& argument) -> bool;
    static auto isConstantArgument(ThreeAddressCode::OptionalArgument const& argument) -> bool;
    static auto isConstantStoreExpression(ThreeAddressCode const& statement) -> bool;
    static auto isConstantBinaryExpression(ThreeAddressCode const& statement) -> bool;
    static auto isBinaryOperation(IRByteCode op) noexcept -> bool;

private:
    IRFunction& m_mainScope;
};
}  // namespace tcc