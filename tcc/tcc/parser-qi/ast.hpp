#if !defined(BOOST_SPIRIT_MINIC_AST_HPP)
#define BOOST_SPIRIT_MINIC_AST_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <list>
#include <utility>

namespace client {
namespace ast {

/**
 * Used to annotate the AST with the iterator position.
 * This id is used as a key to a map<int, Iterator> (not really part of the AST.)
 */
struct tagged {
  int id;
};

struct Nil {};
struct Unary;
struct FunctionCall;
struct expression;

struct Identifier : tagged {
  Identifier(std::string n = "") : name(std::move(n)) {}
  std::string name;
};

using operand = boost::variant<              //
    Nil,                                     //
    bool,                                    //
    unsigned int,                            //
    Identifier,                              //
    boost::recursive_wrapper<Unary>,         //
    boost::recursive_wrapper<FunctionCall>,  //
    boost::recursive_wrapper<expression>     //
    >;

enum optoken {
  op_plus,
  op_minus,
  op_times,
  op_divide,
  op_positive,
  op_negative,
  op_not,
  op_equal,
  op_not_equal,
  op_less,
  op_less_equal,
  op_greater,
  op_greater_equal,
  op_and,
  op_or
};

struct Unary {
  optoken operator_;
  operand operand_;
};

struct operation {
  optoken operator_;
  operand operand_;
};

struct FunctionCall {
  Identifier function_name;
  std::list<expression> args;
};

struct expression {
  operand first;
  std::list<operation> rest;
};

struct Assignment {
  Identifier lhs;
  expression rhs;
};

struct VariableDeclaration {
  Identifier lhs;
  boost::optional<expression> rhs;
};

struct IfStatement;
struct WhileStatement;
struct StatementList;
struct ReturnStatement;

using Statement = boost::variant<               //
    VariableDeclaration,                        //
    Assignment,                                 //
    boost::recursive_wrapper<IfStatement>,      //
    boost::recursive_wrapper<WhileStatement>,   //
    boost::recursive_wrapper<ReturnStatement>,  //
    boost::recursive_wrapper<StatementList>     //
    >;

struct StatementList : std::list<Statement> {};

struct IfStatement {
  expression condition;
  Statement then;
  boost::optional<Statement> else_;
};

struct WhileStatement {
  expression condition;
  Statement body;
};

struct ReturnStatement : tagged {
  boost::optional<expression> expr;
};

struct function {
  std::string return_type;
  Identifier function_name;
  std::list<Identifier> args;
  StatementList body;
};

using FunctionList = std::list<function>;

// print functions for debugging
inline std::ostream& operator<<(std::ostream& out, Nil) {
  out << "Nil";
  return out;
}

inline std::ostream& operator<<(std::ostream& out, Identifier const& id) {
  out << id.name;
  return out;
}
}  // namespace ast
}  // namespace client

BOOST_FUSION_ADAPT_STRUCT(             //
    client::ast::Unary,                //
    (client::ast::optoken, operator_)  //
    (client::ast::operand, operand_)   //
)

BOOST_FUSION_ADAPT_STRUCT(             //
    client::ast::operation,            //
    (client::ast::optoken, operator_)  //
    (client::ast::operand, operand_)   //
)

BOOST_FUSION_ADAPT_STRUCT(                      //
    client::ast::FunctionCall,                  //
    (client::ast::Identifier, function_name)    //
    (std::list<client::ast::expression>, args)  //
)

BOOST_FUSION_ADAPT_STRUCT(                     //
    client::ast::expression,                   //
    (client::ast::operand, first)              //
    (std::list<client::ast::operation>, rest)  //
)

BOOST_FUSION_ADAPT_STRUCT(                           //
    client::ast::VariableDeclaration,                //
    (client::ast::Identifier, lhs)                   //
    (boost::optional<client::ast::expression>, rhs)  //
)

BOOST_FUSION_ADAPT_STRUCT(          //
    client::ast::Assignment,        //
    (client::ast::Identifier, lhs)  //
    (client::ast::expression, rhs)  //
)

BOOST_FUSION_ADAPT_STRUCT(                            //
    client::ast::IfStatement,                         //
    (client::ast::expression, condition)              //
    (client::ast::Statement, then)                    //
    (boost::optional<client::ast::Statement>, else_)  //
)

BOOST_FUSION_ADAPT_STRUCT(                //
    client::ast::WhileStatement,          //
    (client::ast::expression, condition)  //
    (client::ast::Statement, body)        //
)

BOOST_FUSION_ADAPT_STRUCT(                            //
    client::ast::ReturnStatement,                     //
    (boost::optional<client::ast::expression>, expr)  //
)

BOOST_FUSION_ADAPT_STRUCT(                      //
    client::ast::function,                      //
    (std::string, return_type)                  //
    (client::ast::Identifier, function_name)    //
    (std::list<client::ast::Identifier>, args)  //
    (client::ast::StatementList, body)          //
)

#endif
