#pragma once

#include "ast.hpp"
#include "error_handler.hpp"

#include "tcc/compiler/ast.hpp"

#include <ios>
#include <iostream>
#include <map>
#include <vector>

namespace client
{
namespace code_gen
{

struct ssa_builder
{
    std::vector<tcc::Statement::Ptr> statementList {};
};
struct printer
{
    struct ParseResult
    {
        bool success;
        explicit operator bool() const { return success; }
    };

    // using result_type        = bool;
    using result_type        = ParseResult;
    using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

    template<typename ErrorHandler>
    printer(ssa_builder& ssa, ErrorHandler const& error_handler)
        : ssaBuilder(ssa)
        , error_handler([&](x3::position_tagged pos, std::string const& msg) { error_handler(pos, msg); })
    {
    }

    ParseResult start(ast::statement_list const& x) const
    {
        if (!(*this)(x))
        {
            return {false};
        }
        return {true};
    }

    ParseResult operator()(unsigned int x) const
    {
        std::cout << x << " ";
        return {true};
    }

    ParseResult operator()(bool x) const
    {
        std::cout << std::boolalpha << x << " ";
        return {true};
    }

    ParseResult operator()(ast::variable const& x) const
    {
        std::cout << "var: " << x.name << '\n';
        return {true};
    }

    ParseResult operator()(ast::operation const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_)) return {false};
        switch (x.operator_)
        {
            case ast::op_plus: std::cout << "+ "; break;
            case ast::op_minus: std::cout << "- "; break;
            case ast::op_times: std::cout << "* "; break;
            case ast::op_divide: std::cout << "/ "; break;

            case ast::op_equal: std::cout << "== "; break;
            case ast::op_not_equal: std::cout << "!= "; break;
            case ast::op_less: std::cout << "< "; break;
            case ast::op_less_equal: std::cout << "<= "; break;
            case ast::op_greater: std::cout << "> "; break;
            case ast::op_greater_equal: std::cout << ">= "; break;

            case ast::op_and: std::cout << "& "; break;
            case ast::op_or: std::cout << "| "; break;
            default: BOOST_ASSERT(0); return {false};
        }

        return {true};
    }

    ParseResult operator()(ast::unary const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_)) return {false};
        std::cout << "unary: ";
        return {true};
    }

    ParseResult operator()(ast::expression const& x) const
    {
        for (ast::operation const& oper : x.rest)
        {
            if (!(*this)(oper)) return {false};
        }
        if (!boost::apply_visitor(*this, x.first)) return {false};
        return {true};
    }

    ParseResult operator()(ast::assignment const& x) const
    {
        if (!(*this)(x.rhs)) return {false};
        auto const lhs = x.lhs.name;
        std::cout << lhs << " " << '\n';
        return {true};
    }

    ParseResult operator()(ast::variable_declaration const& x) const
    {

        auto const lhs = x.assign.lhs.name;
        std::cout << "var " << lhs << "= ";
        auto r          = (*this)(x.assign.rhs);
        auto expression = std::make_unique<tcc::BinaryExpression>(  //
            std::make_unique<tcc::LiteralExpression>(5),            //
            std::make_unique<tcc::LiteralExpression>(5),            //
            tcc::BinaryExpression::Type::Add                        //
        );

        auto declaration = std::make_unique<tcc::AssignmentStatement>(  //
            lhs,                                                        //
            std::move(expression),
            true  //
        );

        // don't add the variable if the RHS fails
        if (r)
        {
            ssaBuilder.statementList.push_back(std::move(declaration));
        }

        std::cout << '\n';
        return r;
    }

    ParseResult operator()(ast::statement_list const& x) const
    {
        for (auto const& s : x)
        {
            if (!(*this)(s)) return {false};
        }
        return {true};
    }

    ParseResult operator()(ast::statement const& x) const { return boost::apply_visitor(*this, x); }

    ParseResult operator()(ast::if_statement const& x) const { return {true}; }

    ParseResult operator()(ast::while_statement const& x) const { return {true}; }
    ParseResult operator()(ast::nil) const
    {
        BOOST_ASSERT(0);
        return {false};
    }

    ssa_builder& ssaBuilder;
    error_handler_type error_handler;
};
}  // namespace code_gen
}  // namespace client