#if !defined(TCC_PARSER_QI_AST_HPP)
#define TCC_PARSER_QI_AST_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <list>
#include <utility>

namespace tcc {
namespace ast {

/**
 * Used to annotate the AST with the iterator position.
 * This id is used as a key to a map<int, Iterator> (not really part of the AST.)
 */
struct Tagged {
  int id{};
};

struct Nil {};
struct Unary;
struct FunctionCall;
struct Expression;

struct Identifier : Tagged {
  Identifier(std::string n = "") : name(std::move(n)) {}
  std::string name;
};

using Operand = boost::variant<              //
    Nil,                                     //
    bool,                                    //
    unsigned int,                            //
    Identifier,                              //
    boost::recursive_wrapper<Unary>,         //
    boost::recursive_wrapper<FunctionCall>,  //
    boost::recursive_wrapper<Expression>     //
    >;

enum class OpToken {
  Plus,
  Minus,
  Times,
  Divide,
  Positive,
  Negative,
  Not,
  Equal,
  NotEqual,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  And,
  Or
};

struct Unary {
  OpToken operator_;
  Operand operand;
};

struct Operation {
  OpToken operator_;
  Operand operand;
};

struct FunctionCall {
  Identifier function_name;
  std::list<Expression> args;
};

struct Expression {
  Operand first;
  std::list<Operation> rest;
};

struct Assignment {
  Identifier lhs;
  Expression rhs;
};

struct VariableDeclaration {
  Identifier lhs;
  boost::optional<Expression> rhs;
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
  Expression condition;
  Statement then;
  boost::optional<Statement> else_;
};

struct WhileStatement {
  Expression condition;
  Statement body;
};

struct ReturnStatement : Tagged {
  boost::optional<Expression> expr;
};

struct Function {
  std::string return_type;
  Identifier function_name;
  std::list<Identifier> args;
  StatementList body;
};

using FunctionList = std::list<Function>;

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
}  // namespace tcc

BOOST_FUSION_ADAPT_STRUCT(          //
    tcc::ast::Unary,                //
    (tcc::ast::OpToken, operator_)  //
    (tcc::ast::Operand, operand)    //
)

BOOST_FUSION_ADAPT_STRUCT(          //
    tcc::ast::Operation,            //
    (tcc::ast::OpToken, operator_)  //
    (tcc::ast::Operand, operand)    //
)

BOOST_FUSION_ADAPT_STRUCT(                   //
    tcc::ast::FunctionCall,                  //
    (tcc::ast::Identifier, function_name)    //
    (std::list<tcc::ast::Expression>, args)  //
)

BOOST_FUSION_ADAPT_STRUCT(                  //
    tcc::ast::Expression,                   //
    (tcc::ast::Operand, first)              //
    (std::list<tcc::ast::Operation>, rest)  //
)

BOOST_FUSION_ADAPT_STRUCT(                        //
    tcc::ast::VariableDeclaration,                //
    (tcc::ast::Identifier, lhs)                   //
    (boost::optional<tcc::ast::Expression>, rhs)  //
)

BOOST_FUSION_ADAPT_STRUCT(       //
    tcc::ast::Assignment,        //
    (tcc::ast::Identifier, lhs)  //
    (tcc::ast::Expression, rhs)  //
)

BOOST_FUSION_ADAPT_STRUCT(                         //
    tcc::ast::IfStatement,                         //
    (tcc::ast::Expression, condition)              //
    (tcc::ast::Statement, then)                    //
    (boost::optional<tcc::ast::Statement>, else_)  //
)

BOOST_FUSION_ADAPT_STRUCT(             //
    tcc::ast::WhileStatement,          //
    (tcc::ast::Expression, condition)  //
    (tcc::ast::Statement, body)        //
)

BOOST_FUSION_ADAPT_STRUCT(                         //
    tcc::ast::ReturnStatement,                     //
    (boost::optional<tcc::ast::Expression>, expr)  //
)

BOOST_FUSION_ADAPT_STRUCT(                   //
    tcc::ast::Function,                      //
    (std::string, return_type)               //
    (tcc::ast::Identifier, function_name)    //
    (std::list<tcc::ast::Identifier>, args)  //
    (tcc::ast::StatementList, body)          //
)

#endif
