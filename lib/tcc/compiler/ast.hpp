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
    virtual Integer GetResult()                 = 0;
    virtual InstructionList GetAssembly() const = 0;
};

class ConstantExpression : public Expression
{
public:
    ConstantExpression(Integer val) : value(val){};
    ~ConstantExpression() override = default;

    Integer GetResult() override { return value; }
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

    Integer GetResult() override
    {
        switch (m_type)
        {
            case Type::Add: return left->GetResult() + right->GetResult();
            case Type::Subtract: return left->GetResult() - right->GetResult();
            case Type::Multiply: return left->GetResult() * right->GetResult();
            case Type::Divide: return left->GetResult() / right->GetResult();
            default: return 0;
        }
    }

    InstructionList GetAssembly() const override
    {
        auto result = InstructionList{};

        auto const leftAssembly = left->GetAssembly();
        for (auto const& x : leftAssembly) result.push_back(x);

        auto const rightAssembly = right->GetAssembly();
        for (auto const& x : rightAssembly) result.push_back(x);

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

}  // namespace tcc