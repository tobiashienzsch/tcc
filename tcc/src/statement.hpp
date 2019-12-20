#pragma once

#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>

namespace tcc
{

namespace x3 = boost::spirit::x3;

namespace parser
{

struct statement_class;
using statement_type = x3::rule<statement_class, ast::statement_list>;
using statement_id   = statement_type::id;
BOOST_SPIRIT_DECLARE(statement_type);

}  // namespace parser

parser::statement_type const& statement();

}  // namespace tcc
