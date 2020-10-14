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

        errorHandler_ = function<ErrorHandler>(errorHandler)(
            "Error! ", _2, phx::cref(errorHandler.GetIterators())[_1]);
    }

    bool operator()(unsigned int x);

    bool operator()(bool x);

    bool operator()(tcc::ast::Nil x);

    bool operator()(tcc::ast::Identifier const& x);

    bool operator()(tcc::ast::Operation const& x);

    bool operator()(tcc::ast::Unary const& x);

    bool operator()(tcc::ast::FunctionCall const& call);

    bool operator()(tcc::ast::Expression const& x);

    bool operator()(tcc::ast::Assignment const& x);

    bool operator()(tcc::ast::VariableDeclaration const& x);

    bool operator()(tcc::ast::StatementList const& x);

    bool operator()(tcc::ast::Statement const& x);

    bool operator()(tcc::ast::IfStatement const& x);

    bool operator()(tcc::ast::WhileStatement const& x);

    bool operator()(tcc::ast::ReturnStatement const& x);

    bool operator()(tcc::ast::Function const& func);

    bool operator()(tcc::ast::FunctionList const& funcList);

    auto CurrentPackage() -> IRPackage& { return builder_.CurrentPackage(); }

private:
    struct Builder
    {
        Builder() = default;

        [[nodiscard]] auto CurrentPackage() -> IRPackage& { return package_; }

        [[nodiscard]] auto HasVariable(std::string const& name) const -> bool;

        auto PushToStack(std::uint32_t x) -> void;

        [[nodiscard]] auto PopFromStack() -> IRStatement::Argument;

        auto AddVariable(const std::string& name) -> void;

        [[nodiscard]] auto GetLastVariable(std::string const& key) const -> std::string;

        auto CreateReturnOperation() -> void;

        auto CreateBinaryOperation(IRByteCode op) -> void;

        auto CreateUnaryOperation(IRByteCode op) -> void;

        auto CreateStoreOperation(std::string key) -> void;

        auto CreateLoadOperation(std::string key) -> void;

        auto CreateArgStoreOperation(std::string key, std::string varName) -> void;

        [[nodiscard]] auto CreateAssignment(std::string const& key) -> std::string;

        [[nodiscard]] auto CreateTemporaryOnStack() -> std::string;

        [[nodiscard]] auto CreateFunction(std::string name, const IRArgumentList& argsV) -> bool;

        [[nodiscard]] auto CreateFunctionCall(std::string name, IRArgumentList argTemps) -> bool;

        auto CreateIfStatementCondition() -> void;

        auto StartBasicBlock(const std::string& suffix = "") -> void;

        [[nodiscard]] auto GetLastTemporary() const -> std::string;

    private:
        int tmpCounter_   = 0;
        int blockCounter_ = 0;
        std::vector<IRStatement::Argument> stack_;
        IRPackage package_ {"program"};
        IRFunction* currentFunction_ = nullptr;
        IRBasicBlock* currentBlock_  = nullptr;
    };

    Builder builder_ {};
    boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
