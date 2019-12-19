#pragma once

#include "ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(client::ast::unary, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(client::ast::variable_declaration, assign)
BOOST_FUSION_ADAPT_STRUCT(client::ast::assignment, lhs, rhs)
BOOST_FUSION_ADAPT_STRUCT(client::ast::if_statement, condition, then, else_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::while_statement, condition, body)
