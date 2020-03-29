#pragma once

#include "tcc-neo/parser/common.hpp"


namespace tcc
{
namespace parser
{

struct identifier_class;
using identifier_type            = x3::rule<identifier_class, std::string>;
identifier_type const identifier = "identifier";

auto const identifier_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

BOOST_SPIRIT_DEFINE(identifier);

}  // namespace parser
}  // namespace tcc
