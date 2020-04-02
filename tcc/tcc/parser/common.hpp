#pragma once

#include <boost/spirit/home/x3.hpp>

namespace tcc {
namespace parser {
namespace x3 = boost::spirit::x3;

using x3::alnum;
using x3::alpha;
using x3::lexeme;
using x3::raw;

struct identifier_class;
using identifier_type = x3::rule<identifier_class, std::string>;
identifier_type const identifier = "identifier";

auto const identifier_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

BOOST_SPIRIT_DEFINE(identifier)

}  // namespace parser
}  // namespace tcc
