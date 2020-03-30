#pragma once

#include "tcc-neo/parser/common.hpp"
#include "tcc-neo/parser/error_handler.hpp"
#include "tcc-neo/parser/package.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace tcc
{
namespace parser
{
using x3::lexeme;
using x3::raw;
using namespace x3::ascii;

auto const PackageDeclaration = PackageDeclaration_type("PackageDeclaration");
auto const PackageDeclaration_def = PackageDeclaration;
BOOST_SPIRIT_DEFINE(PackageDeclaration);

struct PackageDeclaration_class
    : error_handler_base
    , x3::annotate_on_success
{
};

}  // namespace parser
}  // namespace tcc

namespace tcc
{
parser::PackageDeclaration_type const& PackageDeclaration() { return parser::PackageDeclaration; }
}  // namespace tcc
