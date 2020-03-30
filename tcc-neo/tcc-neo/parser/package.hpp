#pragma once

#include "tcc-neo/parser/identifier.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>

namespace tcc::parser
{

// using functionDefinition_t    = std::vector<identifier_t>;
// auto const functionDefinition = x3::rule<class functionDefinition, functionDefinition_t>()
//     = x3::lit("auto")                    //
//       >> identifier                      //
//       >> x3::lit("(")                    //
//       >> *(identifier % ',')             //
//       >> x3::lit(")")                    //
//       >> -(x3::lit("->") >> identifier)  //
//       >> x3::lit("{}")                   //
//     ;                                    //

// using variableDefinition_t    = std::vector<identifier_t>;
// auto const variableDefinition = x3::rule<class variableDefinition, variableDefinition_t>()
//     = x3::lit("auto")           //
//       >> identifier             //
//       >> x3::lit("=")           //
//       >> x3::lit("expression")  //
//       >> x3::lit(";")           //
//     ;                           //

using packageDeclaration_t    = std::string;
auto const packageDeclaration = x3::rule<class packageDeclaration, packageDeclaration_t>()  //
    = x3::lit("package")                                                                    //
      >> identifier                                                                         //
      >> x3::lit(";")                                                                       //
    ;                                                                                       //

// using packageDefinition_t    = std::tuple<packageDeclaration_t, functionDefinition_t>;
// auto const packageDefinition = x3::rule<class packageDefinition, packageDefinition_t>()
//     = packageDeclaration                             //
//       >> +(variableDefinition | functionDefinition)  //
//     ;                                                //

}  // namespace tcc::parser