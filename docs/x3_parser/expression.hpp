#pragma once

#include <boost/spirit/home/x3.hpp>

#include "tcc/parser/ast.hpp"

namespace tcc {
namespace x3 = boost::spirit::x3;
namespace parser {
struct Expression_class;
using Expression_type = x3::rule<Expression_class, ast::Expression>;
BOOST_SPIRIT_DECLARE(Expression_type)
}  // namespace parser

auto Expression() -> parser::Expression_type const&;
}  // namespace tcc
