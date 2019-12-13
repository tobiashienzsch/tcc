#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "tcc/vm/vm.hpp"

namespace tcc
{
using InstructionList = std::vector<int64_t>;
class Expression
{
public:
    Expression()                          = default;
    virtual ~Expression()                 = default;
    virtual int64_t GetResult()           = 0;
    virtual InstructionList GetAssembly() = 0;
};

class ConstantExpression : public Expression
{
public:
    ConstantExpression(int64_t val) : value(val){};
    ~ConstantExpression() override = default;

    int64_t GetResult() override { return value; }
    InstructionList GetAssembly() override { return {ByteCode::ICONST, value}; }

private:
    int64_t value;
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
    };

public:
    BinaryExpression(Expression* l, Expression* r, Type type) : left(l), right(r), m_type(type) {}
    ~BinaryExpression() override = default;

    int64_t GetResult() override
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

    InstructionList GetAssembly() override
    {
        switch (m_type)
        {
            case Type::Add:
            {
                auto result             = InstructionList{};
                auto const leftAssembly = left->GetAssembly();
                for (auto const& x : leftAssembly) result.push_back(x);
                auto const rightAssembly = right->GetAssembly();
                for (auto const& x : rightAssembly) result.push_back(x);
                result.push_back(ByteCode::IADD);
                return result;
            }

            case Type::Subtract:
            {
                auto result             = InstructionList{};
                auto const leftAssembly = left->GetAssembly();
                for (auto const& x : leftAssembly) result.push_back(x);
                auto const rightAssembly = right->GetAssembly();
                for (auto const& x : rightAssembly) result.push_back(x);
                result.push_back(ByteCode::ISUB);
                return result;
            }

            // case Type::Multiply: return left->GetResult() * right->GetResult();
            // case Type::Divide: return left->GetResult() / right->GetResult();
            default: return {};
        }
    }

private:
    Expression* left;
    Expression* right;
    Type m_type;
};

}  // namespace tcc