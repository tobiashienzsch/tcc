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
    Expression()                                = default;
    virtual ~Expression()                       = default;
    virtual InstructionList GetAssembly() const = 0;
};

Integer AppendExpression(InstructionList& dest, Expression* source);

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
    BinaryExpression(Expression* l, Expression* r, Type type) : left(l), right(r), m_type(type) {}
    ~BinaryExpression() override = default;

    InstructionList GetAssembly() const override
    {
        auto result = InstructionList{};

        AppendExpression(result, left);
        AppendExpression(result, right);

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
    Expression* left;
    Expression* right;
    Type m_type;
};

class TenerayExpression : public Expression
{
public:
    TenerayExpression(Expression* cond, Expression* t, Expression* f) : condition(cond), trueCase(t), falseCase(f){};
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
        AppendExpression(result, condition);
        result.push_back(ByteCode::BRT);

        auto const placeholderIdx = result.size();
        result.push_back(-9999);

        AppendExpression(result, falseCase);

        auto const trueCaseIdx = result.size();
        AppendExpression(result, trueCase);

        result.at(placeholderIdx) = trueCaseIdx;

        return result;
    }

private:
    Expression* condition;
    Expression* trueCase;
    Expression* falseCase;
};

}  // namespace tcc