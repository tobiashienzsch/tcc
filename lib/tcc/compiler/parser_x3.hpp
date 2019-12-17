#pragma once

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

namespace x3 = boost::spirit::x3;

auto const identifier = x3::rule<class identifier, std::string>()  //
    = x3::raw[                                                     //
        x3::lexeme[                                                //
            (x3::alpha | '_')                                      //
            >> *(x3::alnum | '_')                                  //
]                                                                  //
];                                                                 //

auto const functionDefinition = x3::rule<class functionDefinition, std::vector<std::string>>()
    = x3::lit("auto")                    //
      >> identifier                      //
      >> x3::lit("(")                    //
      >> *(identifier % ',')             //
      >> x3::lit(")")                    //
      >> -(x3::lit("->") >> identifier)  //
      >> x3::lit("{}")                   //
    ;                                    //

auto const variableDefinition = x3::rule<class variableDefinition, std::vector<std::string>>()
    = x3::lit("auto")           //
      >> identifier             //
      >> x3::lit("=")           //
      >> x3::lit("expression")  //
      >> x3::lit(";")           //
    ;                           //

auto const packageDefinition
    = x3::rule<class packageDefinition, std::tuple<std::string, std::vector<std::vector<std::string>>>>()
    = x3::lit("package")                             //
      >> identifier                                  //
      >> x3::lit(";")                                //
      >> +(variableDefinition | functionDefinition)  //
    ;                                                //

}  // namespace tcc::parser