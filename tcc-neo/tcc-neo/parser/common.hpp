#pragma once

#include <boost/spirit/home/x3.hpp>

namespace tcc
{
namespace parser
{
namespace x3 = boost::spirit::x3;

using x3::alnum;
using x3::alpha;
using x3::lexeme;
using x3::raw;


}  // namespace parser
}  // namespace tcc