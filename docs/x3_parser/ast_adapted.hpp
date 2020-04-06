#pragma once

#include <boost/fusion/include/adapt_struct.hpp>

#include "tcc/parser/ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(tcc::ast::Unary, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::Operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::Expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::VariableDeclaration, assign)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::Assignment, lhs, rhs)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::IfStatement, condition, then, else_)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::WhileStatement, condition, body)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::CompoundStatement, body)
BOOST_FUSION_ADAPT_STRUCT(tcc::ast::ReturnStatement, expression)