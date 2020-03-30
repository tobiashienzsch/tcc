#pragma once

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"

namespace tcc {
namespace x3 = boost::spirit::x3;
namespace parser {
struct expression_class;
using expression_type = x3::rule<expression_class, ast::expression>;
BOOST_SPIRIT_DECLARE(expression_type);
}  // namespace parser

auto expression() -> parser::expression_type const&;
}  // namespace tcc
