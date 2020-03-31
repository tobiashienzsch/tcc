#pragma once

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"

namespace tcc {

namespace x3 = boost::spirit::x3;

namespace parser {

struct Statement_class;
using Statement_type = x3::rule<Statement_class, ast::Statement_list>;
using Statement_id = Statement_type::id;
BOOST_SPIRIT_DECLARE(Statement_type);

}  // namespace parser

parser::Statement_type const& GetStatement();

}  // namespace tcc
