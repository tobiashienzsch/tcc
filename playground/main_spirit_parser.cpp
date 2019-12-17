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

namespace x3 = boost::spirit::x3;

int main()
{

    auto const input = std::string(R"(
        package main;

        auto some_var = expression;

        auto foo_bar(Tbar, _test) -> void {}
        auto no_returnType() {}

        auto main() -> int {}
    )");

    auto iter     = input.begin();
    auto iter_end = input.end();

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

    std::tuple<std::string, std::vector<std::vector<std::string>>> result;

    phrase_parse(iter,      //
                 iter_end,  //

                 // START GRAMMER

                 packageDefinition,  //

                 // END GRAMMER

                 x3::space,  //
                 result      //
    );

    std::cout << "Package: " << std::get<0>(result) << '\n';
    for (auto const& x : std::get<1>(result))
    {
        for (auto const& y : x)
        {
            std::cout << y << '\n';
        }
    }
}