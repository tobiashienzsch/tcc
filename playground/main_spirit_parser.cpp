// #include <boost/config/warning_disable.hpp>
// #include <boost/spirit/home/x3.hpp>
// #include <iostream>
// #include <string>

// namespace x3 = boost::spirit::x3;

// using x3::alnum;
// using x3::alpha;
// using x3::double_;
// using x3::lit;
// using x3::phrase_parse;
// using x3::ascii::space;

// namespace client {
// ///////////////////////////////////////////////////////////////////////////////
// //  Semantic actions
// ////////////////////////////////////////////////////////1///////////////////////
// namespace {
// using x3::_attr;

// auto do_int = [](auto& ctx) {
//   std::cout << "push " << _attr(ctx) << std::endl;
// };
// auto do_add = [] { std::cout << "add\n"; };
// auto do_subt = [] { std::cout << "subtract\n"; };
// auto do_mult = [] { std::cout << "mult\n"; };
// auto do_div = [] { std::cout << "divide\n"; };
// auto do_neg = [] { std::cout << "negate\n"; };
// auto do_identifier = [](auto& ctx) {
//   std::cout << "identifier" << x3::get<std::string>(ctx) << std::endl;
// };
// }  // namespace

// ///////////////////////////////////////////////////////////////////////////////
// //  The calculator grammar
// ///////////////////////////////////////////////////////////////////////////////
// namespace calculator_grammar {
// using x3::char_;
// using x3::uint_;

// x3::rule<class expression> const expression("expression");
// x3::rule<class term> const term("term");
// x3::rule<class factor> const factor("factor");
// x3::rule<class identifier, std::string> const identifier("identifier");

// auto const identifier_def = alpha >> *alnum;

// auto const expression_def =        //
//     term                           //
//     >> *(('+' >> term[do_add])     //
//          | ('-' >> term[do_subt])  //
//          )                         //
//     ;                              //

// auto const term_def =               //
//     factor                          //
//     >> *(('*' >> factor[do_mult])   //
//          | ('/' >> factor[do_div])  //
//          )                          //
//     ;                               //

// auto const factor_def =          //
//     uint_[do_int]                //
//     | identifier[do_identifier]  //
//     | '(' >> expression >> ')'   //
//     | ('-' >> factor[do_neg])    //
//     | ('+' >> factor)            //
//     ;                            //

// BOOST_SPIRIT_DEFINE(  //
//     identifier,       //
//     expression,       //
//     term,             //
//     factor            //
// );                    //

// auto calculator = expression;
// }  // namespace calculator_grammar

// using calculator_grammar::calculator;

// }  // namespace client

// int main() {
//   auto& calc = client::calculator;  // Our grammar
//   auto input = std::string("name +12*2/1");

//   boost::spirit::x3::ascii::space_type space;
//   bool r = phrase_parse(std::begin(input), std::end(input), calc, space);

//   return 0;
// }

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