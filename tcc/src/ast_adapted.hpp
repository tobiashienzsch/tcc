#pragma once

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(tcc::ast::unary, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::variable_declaration, assign)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::assignment, lhs, rhs)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::IfStatement, condition, then, else_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::WhileStatement, condition, body)
