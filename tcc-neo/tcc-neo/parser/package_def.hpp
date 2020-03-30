#pragma once

#include <boost/spirit/home/x3.hpp>

#include "tcc-neo/parser/common.hpp"
#include "tcc-neo/parser/error_handler.hpp"
#include "tcc-neo/parser/package.hpp"
// #include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace tcc {
namespace parser {
using x3::lexeme;
using x3::raw;
using namespace x3::ascii;

namespace x3 = boost::spirit::x3;

auto const PackageDeclaration = PackageDeclaration_type("PackageDeclaration");
auto const PackageDeclaration_def = x3::lit("package")  //
                                    >> identifier       //
                                    >> x3::lit(";");    //
BOOST_SPIRIT_DEFINE(PackageDeclaration);

struct PackageDeclaration_class
// : error_handler_base
// : x3::annotate_on_success
{};

}  // namespace parser
parser::PackageDeclaration_type const& PackageDeclaration() {
  return parser::PackageDeclaration;
}
}  // namespace tcc
