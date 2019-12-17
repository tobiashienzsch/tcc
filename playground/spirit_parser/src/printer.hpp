#pragma once

#include "ast.hpp"
#include "error_handler.hpp"

#include "tcc/compiler/ast.hpp"

#include <ios>
#include <iostream>
#include <map>
#include <variant>
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

    using result_type        = bool;
    using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

    template<typename ErrorHandler>
    printer(ssa_builder& ssa, ErrorHandler const& error_handler)
        : ssaBuilder(ssa)
        , error_handler([&](x3::position_tagged pos, std::string const& msg) { error_handler(pos, msg); })
    {
    }

    bool start(ast::statement_list const& x) const
    {
        if (!(*this)(x))
        {
            return false;
        }
        return true;
    }

    bool operator()(unsigned int x) const
    {
        std::cout << x << " ";
        return true;
    }

    bool operator()(bool x) const
    {
        std::cout << std::boolalpha << x << " ";
        return true;
    }

    bool operator()(ast::variable const& x) const
    {
        std::cout << "var: " << x.name << '\n';
        return true;
    }

    bool operator()(ast::operation const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_)) return false;
        auto result = tcc::BinaryExpression::Type {};
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
            default: BOOST_ASSERT(0); return false;
        }
        return true;
    }

    bool operator()(ast::unary const& x) const
    {
        if (!boost::apply_visitor(*this, x.operand_)) return false;
        std::cout << "unary: ";
        return true;
    }

    bool operator()(ast::expression const& x) const
    {
        auto lhs = boost::apply_visitor(*this, x.first);
        if (!lhs) return false;
        for (ast::operation const& oper : x.rest)
        {
            if (!(*this)(oper)) return false;
        }
        return true;
    }

    bool operator()(ast::assignment const& x) const
    {
        if (!(*this)(x.rhs)) return false;
        auto const lhs = x.lhs.name;
        std::cout << lhs << " " << '\n';
        return true;
    }

    bool operator()(ast::variable_declaration const& x) const
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

    bool operator()(ast::statement_list const& x) const
    {
        for (auto const& s : x)
        {
            if (!(*this)(s)) return false;
        }
        return true;
    }

    bool operator()(ast::statement const& x) const
    {
        tcc::Expression* expression = nullptr;
        return boost::apply_visitor(*this, x);
    }

    bool operator()(ast::if_statement const& x) const { return true; }

    bool operator()(ast::while_statement const& x) const { return true; }
    bool operator()(ast::nil) const
    {
        BOOST_ASSERT(0);
        return false;
    }

    ssa_builder& ssaBuilder;
    error_handler_type error_handler;
};
}  // namespace code_gen
}  // namespace client