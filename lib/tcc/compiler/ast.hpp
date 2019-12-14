#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "tcc/vm/vm.hpp"

namespace tcc
{
using InstructionList = std::vector<Integer>;

class Expression
{
public:
    using Ptr = std::unique_ptr<Expression const>;

public:
    Expression()                                = default;
    virtual ~Expression()                       = default;
    virtual InstructionList GetAssembly() const = 0;
};

Integer AppendExpression(InstructionList& dest, Expression const& source);

class LiteralExpression : public Expression
{
public:
    LiteralExpression(Integer val) : value(val){};
    ~LiteralExpression() override = default;

    InstructionList GetAssembly() const override { return {ByteCode::ICONST, value}; }

private:
    Integer value;
};

class BinaryExpression : public Expression
{
public:
    enum class Type
    {
        Add,
        Subtract,
        Multiply,
        Divide,

        Less,
        Equal,
    };

public:
    BinaryExpression(Expression::Ptr l, Expression::Ptr r, Type type)
        : left(std::move(l)), right(std::move(r)), m_type(type)
    {
    }
    ~BinaryExpression() override = default;

    InstructionList GetAssembly() const override
    {
        auto result = InstructionList{};

        AppendExpression(result, *left.get());
        AppendExpression(result, *right.get());

        switch (m_type)
        {
            case Type::Add:
            {
                result.push_back(ByteCode::IADD);
                return result;
            }

            case Type::Subtract:
            {
                result.push_back(ByteCode::ISUB);
                return result;
            }

            case Type::Multiply:
            {
                result.push_back(ByteCode::ISUB);
                return result;
            }

            case Type::Divide:
            {
                result.push_back(ByteCode::ISUB);
                return result;
            }

            case Type::Less:
            {
                result.push_back(ByteCode::ILT);
                return result;
            }

            case Type::Equal:
            {
                result.push_back(ByteCode::IEQ);
                return result;
            }

            default: return {};
        }
    }

private:
    Expression::Ptr left;
    Expression::Ptr right;
    Type m_type;
};

class TenerayExpression : public Expression
{
public:
    TenerayExpression(Expression::Ptr cond, Expression::Ptr t, Expression::Ptr f)
        : condition(std::move(cond)), trueCase(std::move(t)), falseCase(std::move(f)){};

    ~TenerayExpression() override = default;

    InstructionList GetAssembly() const override
    {
        // true ? print(42) : print(2)
        // .condition
        // ICONST 1         // 0
        // BRT 7            // 2

        // .false case
        // ICONST 2         // 4
        // PRINT            // 6

        // .true case
        // ICONST 42        // 7
        // PRINT            // 9

        auto result = InstructionList{};
        AppendExpression(result, *condition.get());
        result.push_back(ByteCode::BRT);

        auto const placeholderIdx = result.size();
        result.push_back(0);

        AppendExpression(result, *falseCase.get());

        auto const trueCaseIdx = result.size();
        AppendExpression(result, *trueCase.get());

        result.at(placeholderIdx) = trueCaseIdx;

        return result;
    }

private:
    Expression::Ptr condition;
    Expression::Ptr trueCase;
    Expression::Ptr falseCase;
};

class Statement
{
public:
    using Ptr = std::unique_ptr<Statement const>;

    enum class Type
    {
        Compound,
        Expression,

        Conditional,
        Loop,

        VariableDefinition,

        Return,
        Noop,
    };

public:
    Statement()                                                         = default;
    virtual ~Statement()                                                = default;
    virtual Type GetType() const                                        = 0;
    virtual InstructionList GetAssembly(Integer const offset = 0) const = 0;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(Expression::Ptr exp) : expression(std::move(exp)) {}
    ~ExpressionStatement() override = default;

    Statement::Type GetType() const noexcept override { return Statement::Type::Expression; };
    InstructionList GetAssembly(Integer const offset = 0) const override { return expression->GetAssembly(); };

private:
    Expression::Ptr expression;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement(Expression::Ptr exp) : expression(std::move(exp)) {}
    ~ReturnStatement() override = default;

    Statement::Type GetType() const noexcept override { return Statement::Type::Return; };
    InstructionList GetAssembly(Integer const offset = 0) const override
    {
        auto result = expression->GetAssembly();
        result.push_back(ByteCode::RET);
        return result;
    };

private:
    Expression::Ptr expression;
};

class ConditionalStatement : public Statement
{
public:
    ConditionalStatement(Expression::Ptr cond, Statement::Ptr trueCase)
        : m_condition(std::move(cond)), m_trueCase(std::move(trueCase))
    {
    }

    ~ConditionalStatement() override = default;

    Statement::Type GetType() const noexcept override { return Statement::Type::Conditional; };

    InstructionList GetAssembly(Integer const offset = 0) const override
    {
        auto result = m_condition->GetAssembly();
        result.push_back(ByteCode::BRF);

        auto const trueCaseAsm = m_trueCase->GetAssembly();
        result.push_back(trueCaseAsm.size() + result.size() + 1 + offset);
        result.insert(std::end(result), std::begin(trueCaseAsm), std::end(trueCaseAsm));

        return result;
    };

private:
    Expression::Ptr m_condition;
    Statement::Ptr m_trueCase;
};

class CompoundStatement : public Statement
{
public:
    template<typename... SatementPointer>
    explicit CompoundStatement(SatementPointer... statement)
    {
        // static_assert((std::is_constructible_v<T, Args&&> && ...));
        (m_statements.push_back(std::move(statement)), ...);
    }

    ~CompoundStatement() override = default;

    Statement::Type GetType() const noexcept override { return Statement::Type::Compound; };

    InstructionList GetAssembly(Integer const offset = 0) const override
    {
        auto result         = InstructionList{};
        auto internalOffset = offset;

        for (Statement::Ptr const& statement : m_statements)
        {
            auto const assembly = statement->GetAssembly(internalOffset);
            internalOffset += assembly.size();

            result.insert(std::end(result), std::begin(assembly), std::end(assembly));
        }

        return result;
    };

private:
    std::vector<Statement::Ptr> m_statements;
};

}  // namespace tcc