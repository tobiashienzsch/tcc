#pragma once

#include <boost/spirit/home/x3.hpp>

#include "tcc/parser/error_handler.hpp"

namespace tcc {
namespace parser {

using iterator_type = std::string::const_iterator;
using phrase_context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
using error_handler_type = error_handler<iterator_type>;
using context_type = x3::context<error_handler_tag, std::reference_wrapper<error_handler_type>, phrase_context_type>;

}  // namespace parser
}  // namespace tcc
