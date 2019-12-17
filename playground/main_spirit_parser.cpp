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
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <iostream>
#include <list>
#include <numeric>
#include <string>

namespace x3 = boost::spirit::x3;

namespace client
{
namespace ast
{
///////////////////////////////////////////////////////////////////////////
//  The AST
///////////////////////////////////////////////////////////////////////////
struct nil
{
};
struct signed_;
struct program;

struct operand : x3::variant<nil, unsigned int, x3::forward_ast<signed_>, x3::forward_ast<program>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct signed_
{
    char sign;
    operand operand_;
};

struct operation
{
    char operator_;
    operand operand_;
};

struct program
{
    operand first;
    std::list<operation> rest;
};
}  // namespace ast
}  // namespace client

BOOST_FUSION_ADAPT_STRUCT(client::ast::signed_, sign, operand_)

BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, operator_, operand_)

BOOST_FUSION_ADAPT_STRUCT(client::ast::program, first, rest)

namespace client
{
namespace ast
{
///////////////////////////////////////////////////////////////////////////
//  The AST Printer
///////////////////////////////////////////////////////////////////////////
struct printer
{
    typedef void result_type;

    void operator()(nil) const {}
    void operator()(unsigned int n) const { std::cout << n; }

    void operator()(operation const& x) const
    {
        boost::apply_visitor(*this, x.operand_);
        switch (x.operator_)
        {
            case '+': std::cout << " add"; break;
            case '-': std::cout << " subt"; break;
            case '*': std::cout << " mult"; break;
            case '/': std::cout << " div"; break;
        }
    }

    void operator()(signed_ const& x) const
    {
        boost::apply_visitor(*this, x.operand_);
        switch (x.sign)
        {
            case '-': std::cout << " neg"; break;
            case '+': std::cout << " pos"; break;
        }
    }

    void operator()(program const& x) const
    {
        boost::apply_visitor(*this, x.first);
        for (operation const& oper : x.rest)
        {
            std::cout << ' ';
            (*this)(oper);
        }
    }
};

///////////////////////////////////////////////////////////////////////////
//  The AST evaluator
///////////////////////////////////////////////////////////////////////////
struct eval
{
    typedef int result_type;

    int operator()(nil) const
    {
        BOOST_ASSERT(0);
        return 0;
    }
    int operator()(unsigned int n) const { return n; }

    int operator()(int lhs, operation const& x) const
    {
        int rhs = boost::apply_visitor(*this, x.operand_);
        switch (x.operator_)
        {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '*': return lhs * rhs;
            case '/': return lhs / rhs;
        }
        BOOST_ASSERT(0);
        return 0;
    }

    int operator()(signed_ const& x) const
    {
        int rhs = boost::apply_visitor(*this, x.operand_);
        switch (x.sign)
        {
            case '-': return -rhs;
            case '+': return +rhs;
        }
        BOOST_ASSERT(0);
        return 0;
    }

    int operator()(program const& x) const
    {
        return std::accumulate(x.rest.begin(), x.rest.end(), boost::apply_visitor(*this, x.first), *this);
    }
};
}  // namespace ast
}  // namespace client

namespace client
{

namespace calculator_grammar
{
using x3::char_;
using x3::uint_;

x3::rule<class expression, ast::program> const expression("expression");
x3::rule<class term, ast::program> const term("term");
x3::rule<class factor, ast::operand> const factor("factor");

auto const expression_def =        //
    term                           //
    >> *((char_('+') >> term)      //
         | (char_('-') >> term));  //

auto const term_def =                //
    factor                           //
    >> *((char_('*') >> factor)      //
         | (char_('/') >> factor));  //

auto const factor_def =         //
    uint_                       //
    | '(' >> expression >> ')'  //
    | (char_('-') >> factor)    //
    | (char_('+') >> factor);   //

BOOST_SPIRIT_DEFINE(  //
    expression,       //
    term,             //
    factor            //
);

auto calculator = expression;
}  // namespace calculator_grammar

using calculator_grammar::calculator;

}  // namespace client

int main(int argc, char** argv)
{
    std::string str = "(1+2)*3+5";
    if (argc == 2)
    {
        str = std::string(argv[1]);
    }

    using ast_program = client::ast::program;
    using ast_print   = client::ast::printer;
    using ast_eval    = client::ast::eval;

    auto& calc = client::calculator;  // Our grammar
    ast_program program;              // Our program (AST)
    ast_print print;                  // Prints the program
    ast_eval eval;                    // Evaluates the program

    auto iter = std::begin(str);
    auto end  = std::end(str);
    boost::spirit::x3::ascii::space_type space;
    bool r = phrase_parse(iter, end, calc, space, program);

    if (r && iter == end)
    {
        std::cout << "Parsing succeeded\n";
        print(program);
        std::cout << "\nResult: " << eval(program) << std::endl;
    }
    else
    {
        std::string rest(iter, end);
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \"" << rest << "\"\n";
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}