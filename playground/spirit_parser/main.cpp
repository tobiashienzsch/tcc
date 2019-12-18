#include "ast.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "error_handler.hpp"
#include "printer.hpp"
#include "statement.hpp"
#include "vm.hpp"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main()
{

    std::string str;
    std::string source;
    // while (std::getline(std::cin, str))
    // {
    //     if (str.empty()) break;
    //     source += str + '\n';
    // }

    source = "var x = 1+(2+7*8/2)*3;x=x*2;var y=x+2*2;\n\n";

    using client::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    client::vmachine vm;                // Our virtual machine
    client::code_gen::program program;  // Our VM program
    client::ast::statement_list ast;    // Our AST

    using boost::spirit::x3::with;
    using client::parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

    // Our compiler
    client::code_gen::compiler compile(program, error_handler);
    client::code_gen::ssa_builder ssaBuilder {};
    client::code_gen::printer printer(ssaBuilder, error_handler);

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<client::parser::error_handler_tag>(std::ref(error_handler))[client::statement()];

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success && iter == end)
    {
        // if (compile.start(ast))
        // {
        //     std::cout << "Success\n";
        //     std::cout << "-------------------------\n";
        //     vm.execute(program());

        //     std::cout << "-------------------------\n";
        //     std::cout << "Assembler----------------\n\n";
        //     program.print_assembler();

        //     std::cout << "-------------------------\n";
        //     std::cout << "Results------------------\n\n";
        //     program.print_variables(vm.get_stack());
        // }
        // else
        // {
        //     std::cout << "Compile failure\n";
        // }

        if (printer.start(ast))
        {
            for (auto& x : printer.ssaBuilder.statementList) std::cout << x.get() << '\n';
        }
        else
        {
            std::cout << "Printer failure\n";
        }
    }
    else
    {
        std::cout << "Parse failure\n";
    }

    return 0;
}

// #include <boost/config/warning_disable.hpp>
// #include <boost/fusion/include/adapt_struct.hpp>
// #include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/home/x3/support/ast/variant.hpp>

// #include <iostream>
// #include <list>
// #include <numeric>
// #include <string>

// namespace x3 = boost::spirit::x3;

// namespace tcc
// {
// namespace ast
// {
// ///////////////////////////////////////////////////////////////////////////
// //  The AST
// ///////////////////////////////////////////////////////////////////////////
// struct nil
// {
// };
// struct signed_;
// struct expression;

// struct operand : x3::variant<nil, unsigned int, x3::forward_ast<signed_>, x3::forward_ast<expression>>
// {
//     using base_type::base_type;
//     using base_type::operator=;
// };

// struct signed_
// {
//     char sign;
//     operand operand_;
// };

// struct operation
// {
//     char operator_;
//     operand operand_;
// };

// struct expression
// {
//     operand first;
//     std::list<operation> rest;
// };

// // print function for debugging
// inline std::ostream& operator<<(std::ostream& out, nil)
// {
//     out << "nil";
//     return out;
// }
// }  // namespace ast
// }  // namespace tcc

// BOOST_FUSION_ADAPT_STRUCT(tcc::ast::signed_, sign, operand_)
// BOOST_FUSION_ADAPT_STRUCT(tcc::ast::operation, operator_, operand_)
// BOOST_FUSION_ADAPT_STRUCT(tcc::ast::expression, first, rest)

// namespace tcc
// {
// namespace ast
// {
// ///////////////////////////////////////////////////////////////////////////
// //  The AST Printer
// ///////////////////////////////////////////////////////////////////////////
// struct printer
// {
//     typedef void result_type;

//     void operator()(nil) const {}
//     void operator()(unsigned int n) const { std::cout << n; }

//     void operator()(operation const& x) const
//     {
//         boost::apply_visitor(*this, x.operand_);
//         switch (x.operator_)
//         {
//             case '+': std::cout << " add"; break;
//             case '-': std::cout << " subt"; break;
//             case '*': std::cout << " mult"; break;
//             case '/': std::cout << " div"; break;
//         }
//     }

//     void operator()(signed_ const& x) const
//     {
//         boost::apply_visitor(*this, x.operand_);
//         switch (x.sign)
//         {
//             case '-': std::cout << " neg"; break;
//             case '+': std::cout << " pos"; break;
//         }
//     }

//     void operator()(expression const& x) const
//     {
//         boost::apply_visitor(*this, x.first);
//         for (operation const& oper : x.rest)
//         {
//             std::cout << ' ';
//             (*this)(oper);
//         }
//     }
// };

// ///////////////////////////////////////////////////////////////////////////
// //  The AST evaluator
// ///////////////////////////////////////////////////////////////////////////
// struct eval
// {
//     using result_type = int;

//     result_type operator()(nil) const
//     {
//         BOOST_ASSERT(0);
//         return 0;
//     }
//     result_type operator()(unsigned int n) const { return n; }

//     result_type operator()(operation const& x, int lhs) const
//     {
//         int rhs = boost::apply_visitor(*this, x.operand_);
//         switch (x.operator_)
//         {
//             case '+': return lhs + rhs;
//             case '-': return lhs - rhs;
//             case '*': return lhs * rhs;
//             case '/': return lhs / rhs;
//         }
//         BOOST_ASSERT(0);
//         return 0;
//     }

//     result_type operator()(signed_ const& x) const
//     {
//         int rhs = boost::apply_visitor(*this, x.operand_);
//         switch (x.sign)
//         {
//             case '-': return -rhs;
//             case '+': return +rhs;
//         }
//         BOOST_ASSERT(0);
//         return 0;
//     }

//     result_type operator()(expression const& x) const
//     {
//         int state = boost::apply_visitor(*this, x.first);
//         for (operation const& oper : x.rest)
//         {
//             state = (*this)(oper, state);
//         }
//         return state;
//     }
// };
// }  // namespace ast
// }  // namespace tcc

// namespace tcc
// {
// ///////////////////////////////////////////////////////////////////////////////
// //  The calculator grammar
// ///////////////////////////////////////////////////////////////////////////////
// namespace calculator_grammar
// {
// using x3::char_;
// using x3::uint_;

// struct expression_class;
// struct term_class;
// struct factor_class;

// x3::rule<expression_class, ast::expression> const expression("expression");
// x3::rule<term_class, ast::expression> const term("term");
// x3::rule<factor_class, ast::operand> const factor("factor");

// auto const expression_def = term >> *((char_('+') > term) | (char_('-') > term));

// auto const term_def = factor >> *((char_('*') > factor) | (char_('/') > factor));

// auto const factor_def = uint_ | '(' > expression > ')' | (char_('-') > factor) | (char_('+') > factor);

// BOOST_SPIRIT_DEFINE(expression, term, factor);

// struct expression_class
// {
//     //  Our error handler
//     template<typename Iterator, typename Exception, typename Context>
//     x3::error_handler_result on_error(Iterator&, Iterator const& last, Exception const& x, Context const& context)
//     {
//         std::cout << "Error! Expecting: " << x.which() << " here: \"" << std::string(x.where(), last) << "\""
//                   << std::endl;
//         return x3::error_handler_result::fail;
//     }
// };

// auto calculator = expression;
// }  // namespace calculator_grammar

// using calculator_grammar::calculator;
// }  // namespace tcc

// int main()
// {
//     using ast_expression = tcc::ast::expression;
//     using ast_print      = tcc::ast::printer;
//     using ast_eval       = tcc::ast::eval;

//     std::string str;
//     while (std::getline(std::cin, str))
//     {
//         if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

//         auto& calc   = tcc::calculator;    // Our grammar
//         auto program = ast_expression {};  // Our program (AST)
//         auto print   = ast_print {};       // Prints the program
//         auto eval    = ast_eval {};        // Evaluates the program

//         auto iter        = str.begin();
//         auto end         = str.end();
//         auto space       = boost::spirit::x3::ascii::space_type {};
//         bool parseResult = phrase_parse(iter, end, calc, space, program);

//         if (parseResult && iter == end)
//         {
//             print(program);
//             std::cout << "\nResult: " << eval(program) << '\n';
//         }
//         else
//         {
//             std::cout << "Parsing failed\n";
//         }
//     }

//     return 0;
// }