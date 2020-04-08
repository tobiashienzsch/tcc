#if !defined(TCC_IR_GENERATOR_HPP)
#define TCC_IR_GENERATOR_HPP

#include "tcc/ir/statement.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"

#include <boost/function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace tcc
{

class IRGenerator
{
public:
    using result_type = bool;

    template<typename ErrorHandler>
    IRGenerator(ErrorHandler& errorHandler)
    {
        using namespace boost::phoenix::arg_names;
        namespace phx = boost::phoenix;
        using boost::phoenix::function;

        errorHandler_ = function<ErrorHandler>(errorHandler)("Error! ", _2, phx::cref(errorHandler.GetIterators())[_1]);
    }

    bool operator()(unsigned int x);
    bool operator()(bool x);
    bool operator()(tcc::ast::Nil x);
    bool operator()(tcc::ast::Identifier const& x);
    bool operator()(tcc::ast::Operation const& x);
    bool operator()(tcc::ast::Unary const& x);
    bool operator()(tcc::ast::FunctionCall const& x);
    bool operator()(tcc::ast::Expression const& x);
    bool operator()(tcc::ast::Assignment const& x);
    bool operator()(tcc::ast::VariableDeclaration const& x);
    bool operator()(tcc::ast::StatementList const& x);
    bool operator()(tcc::ast::Statement const& x);
    bool operator()(tcc::ast::IfStatement const& x);
    bool operator()(tcc::ast::WhileStatement const& x);
    bool operator()(tcc::ast::ReturnStatement const& x);
    bool operator()(tcc::ast::Function const& func);
    bool operator()(tcc::ast::FunctionList const& funcs);

    auto CurrentPackage() -> IRPackage& { return builder_.CurrentPackage(); }

private:
    struct Builder
    {
        Builder() = default;

        [[nodiscard]] auto CurrentPackage() -> IRPackage& { return package_; }

        [[nodiscard]] auto HasVariable(std::string const& name) const -> bool
        {
            auto i = currentFunction_->variables.find(name);
            return i != currentFunction_->variables.end();
        }

        auto PushToStack(std::uint32_t x) -> void { stack_.emplace_back(x); }

        [[nodiscard]] auto PopFromStack() -> IRStatement::Argument
        {
            auto const result = stack_.back();
            stack_.pop_back();
            return result;
        }

        auto AddVariable(const std::string& name) -> void
        {
            auto search = currentFunction_->variables.find(name);
            if (search == currentFunction_->variables.end())
            {
                currentFunction_->variables.insert({name, 0});
            }
            else
            {
                fmt::print("Tried to add {} twice to variable map\n", name);
            }
        }

        [[nodiscard]] auto GetLastVariable(std::string const& key) const -> std::string
        {
            auto search = currentFunction_->variables.find(key);
            auto newId  = search->second - 1;
            return fmt::format("{}.{}", key, newId);
        }

        auto CreateReturnOperation() -> void
        {
            auto const first = PopFromStack();
            currentFunction_->statements.emplace_back(
                IRStatement {IRByteCode::Return, "ret", first, std::nullopt, false});
        }

        auto CreateBinaryOperation(IRByteCode op) -> void
        {
            auto const second  = PopFromStack();
            auto const first   = PopFromStack();
            auto const tmpName = CreateTemporaryOnStack();

            currentFunction_->statements.emplace_back(IRStatement {op, tmpName, first, second});
        }

        auto CreateUnaryOperation(IRByteCode op) -> void
        {
            currentFunction_->statements.emplace_back(IRStatement {op, CreateTemporaryOnStack(), PopFromStack(), {}});
        }

        auto CreateStoreOperation(std::string key) -> void
        {
            currentFunction_->statements.emplace_back(
                IRStatement {IRByteCode::Store, std::move(key), PopFromStack(), {}, false});
        }

        auto CreateLoadOperation(std::string key) -> void
        {
            currentFunction_->statements.emplace_back(
                IRStatement {IRByteCode::Load, CreateTemporaryOnStack(), key, {}});
        }

        [[nodiscard]] auto CreateAssignment(std::string const& key) -> std::string
        {
            auto search = currentFunction_->variables.find(key);
            auto newId  = search->second++;
            return fmt::format("{}.{}", key, newId);
        }

        [[nodiscard]] auto CreateTemporaryOnStack() -> std::string
        {
            auto const tmp = fmt::format("t.{}", tmpCounter_++);
            stack_.emplace_back(tmp);
            return tmp;
        }

        [[nodiscard]] auto CreateFunction(std::string name, std::vector<std::string> args) -> bool
        {
            package_.functions.emplace_back(IRFunction {std::move(name), std::move(args), {}, {}});
            currentFunction_ = &package_.functions.back();
            for (auto const& arg : currentFunction_->args)
            {
                AddVariable(arg);
            }

            return true;
        }

        [[nodiscard]] auto CreateFunctionCall(std::string name, size_t numArgs) -> bool
        {
            auto args = std::vector<std::string> {};
            currentFunction_->statements.emplace_back(
                IRStatement {IRByteCode::Call, CreateTemporaryOnStack(), std::move(name), args, {}});
            return true;
        }

    private:
        int tmpCounter_ = 0;
        std::vector<IRStatement::Argument> stack_;
        IRPackage package_ {"program"};
        IRFunction* currentFunction_ = nullptr;
    };

private:
    Builder builder_ {};
    boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
