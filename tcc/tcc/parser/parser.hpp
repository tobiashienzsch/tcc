#pragma once

#include "tcc/parser/function.hpp"
#include "tcc/parser/skipper.hpp"

namespace tcc
{
template<typename IteratorType>
class Parser
{
public:
    using AstType = tcc::ast::FunctionList;

    Parser(tcc::ErrorHandler<IteratorType>& err) : errorHandler_(err) { }

    bool ParseSource(IteratorType& iter, IteratorType& end)
    {
        bool const success = phrase_parse(iter, end, +grammar_, skipper_, ast_);
        return !static_cast<bool>(!success || iter != end);
    }

    AstType& GetAst() noexcept { return ast_; }

private:
    tcc::ErrorHandler<IteratorType>& errorHandler_;
    tcc::parser::Function<IteratorType> grammar_ {errorHandler_};
    tcc::parser::Skipper<IteratorType> skipper_;
    AstType ast_ {};
};
}  // namespace tcc