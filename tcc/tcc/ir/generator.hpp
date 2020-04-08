#if !defined(TCC_IR_GENERATOR_HPP)
#define TCC_IR_GENERATOR_HPP

#include "tcc/ir/three_address_code.hpp"
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
    bool operator()(tcc::ast::Function const& x);
    bool operator()(tcc::ast::FunctionList const& x);

    auto PrintIR() -> void { builder_.PrintIR(); }
    auto CurrentScope() -> StatementScope* { return builder_.CurrentScope(); }

private:
    struct Builder
    {
        Builder() = default;

        auto PrintIR() -> void
        {
            fmt::print("\nprogram: {} IR instructions\n", rootScope_.statements.size());
            fmt::print("func main: args=[]\n");
            fmt::print("entry:\n");
            for (ThreeAddressCode const& x : rootScope_.statements)
            {
                fmt::print("\t{}\n", x);
            }
        }

        [[nodiscard]] auto CurrentScope() -> StatementScope*
        {
            if (currentScope_ == nullptr)
            {
                fmt::print("Current scope is nullptr;\n EXIT\n");
                std::exit(1);
            }

            return currentScope_;
        }

        [[nodiscard]] auto HasVariable(std::string const& name) const -> bool
        {
            auto i = rootScope_.variables.find(name);
            return i != rootScope_.variables.end();
        }

        auto PushToStack(std::uint32_t x) -> void { stack_.emplace_back(x); }

        [[nodiscard]] auto PopFromStack() -> std::variant<std::uint32_t, std::string>
        {
            auto const result = stack_.back();
            stack_.pop_back();
            return result;
        }

        auto AddVariable(const std::string& name) -> void
        {
            auto search = rootScope_.variables.find(name);
            if (search == rootScope_.variables.end())
            {
                rootScope_.variables.insert({name, 0});
            }
            else
            {
                fmt::print("Tried to add {} twice to variable map\n", name);
            }
        }

        [[nodiscard]] auto GetLastVariable(std::string const& key) const -> std::string
        {
            auto search = rootScope_.variables.find(key);
            auto newId  = search->second - 1;
            return fmt::format("{}.{}", key, newId);
        }

        auto CreateReturnOperation() -> void
        {
            auto const first = PopFromStack();
            rootScope_.statements.push_back(ThreeAddressCode {IRByteCode::Return, "g.0", first, std::nullopt, false});
        }

        auto CreateBinaryOperation(IRByteCode op) -> void
        {
            auto const second  = PopFromStack();
            auto const first   = PopFromStack();
            auto const tmpName = CreateTemporaryOnStack();

            rootScope_.statements.push_back(ThreeAddressCode {op, tmpName, first, second});
        }

        auto CreateUnaryOperation(IRByteCode op) -> void
        {
            auto const first   = PopFromStack();
            auto const tmpName = CreateTemporaryOnStack();
            rootScope_.statements.push_back(ThreeAddressCode {op, tmpName, first, {}});
        }

        auto CreateStoreOperation(std::string key) -> void
        {
            auto const first = PopFromStack();
            rootScope_.statements.push_back(ThreeAddressCode {IRByteCode::Store, std::move(key), first, {}, false});
        }

        auto CreateLoadOperation(std::string key) -> void
        {
            auto const tmpName = CreateTemporaryOnStack();
            rootScope_.statements.push_back(ThreeAddressCode {IRByteCode::Load, tmpName, key, {}});
        }

        [[nodiscard]] auto CreateAssignment(std::string const& key) -> std::string
        {
            auto search = rootScope_.variables.find(key);
            auto newId  = search->second++;
            return fmt::format("{}.{}", key, newId);
        }

        [[nodiscard]] auto CreateTemporaryOnStack() -> std::string
        {
            auto tmp = std::string("t.").append(std::to_string(tmpCounter_++));
            stack_.emplace_back(tmp);
            return tmp;
        }

        [[nodiscard]] auto GetStatementList() -> StatementList& { return rootScope_.statements; }

    private:
        int tmpCounter_ = 0;
        std::vector<std::variant<std::uint32_t, std::string>> stack_;
        StatementScope rootScope_ {"main"};
        StatementScope* currentScope_ = &rootScope_;
    };

private:
    Builder builder_ {};
    boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
