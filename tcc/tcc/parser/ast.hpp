#if !defined(TCC_PARSER_QI_AST_HPP)
#define TCC_PARSER_QI_AST_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <list>
#include <utility>

namespace tcc
{
namespace ast
{

/**
 * @brief Used to annotate the AST with the iterator position.
 * This id is used as a key to a map<int, Iterator>.
 * Not really part of the AST.
 */
struct Tagged
{
    int ID {};
};

/**
 * @brief Placeholder for nothing.
 */
struct Nil
{
};

struct Unary;

struct FunctionCall;

struct Expression;

/**
 * @brief Identifier.
 */
struct Identifier : Tagged
{
    Identifier(std::string n = "") : Name(std::move(n)) { }
    std::string Name;
};

/**
 * @brief Operand.
 */
using Operand = boost::variant<              //
    Nil,                                     //
    bool,                                    //
    unsigned int,                            //
    Identifier,                              //
    boost::recursive_wrapper<Unary>,         //
    boost::recursive_wrapper<FunctionCall>,  //
    boost::recursive_wrapper<Expression>     //
    >;

/**
 * @brief Operation Token.
 */
enum class OpToken
{
    Invalid,
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

/**
 * @brief Unary Expression.
 */
struct Unary
{
    OpToken operator_ {OpToken::Invalid};
    Operand operand;
};

/**
 * @brief Operation.
 */
struct Operation
{
    OpToken operator_ {OpToken::Invalid};
    Operand operand;
};

/**
 * @brief Function call.
 */
struct FunctionCall
{
    Identifier FuncName;
    std::list<Expression> args;
};

/**
 * @brief Expression.
 */
struct Expression
{
    Operand first;
    std::list<Operation> rest;
};

/**
 * @brief Assignment.
 */
struct Assignment
{
    Identifier lhs;
    Expression rhs;
};

/**
 * @brief Variable Declaration.
 */
struct VariableDeclaration
{
    Identifier lhs;
    boost::optional<Expression> rhs;
};

struct IfStatement;
struct WhileStatement;
struct StatementList;
struct ReturnStatement;

/**
 * @brief Statement.
 */
using Statement = boost::variant<               //
    VariableDeclaration,                        //
    Assignment,                                 //
    boost::recursive_wrapper<IfStatement>,      //
    boost::recursive_wrapper<WhileStatement>,   //
    boost::recursive_wrapper<ReturnStatement>,  //
    boost::recursive_wrapper<StatementList>     //
    >;

/**
 * @brief Statement list.
 */
struct StatementList : std::list<Statement>
{
};

/**
 * @brief If statement.
 */
struct IfStatement
{
    Expression condition;
    Statement then;
    boost::optional<Statement> else_;
};

/**
 * @brief While statement.
 */
struct WhileStatement
{
    Expression condition;
    Statement body;
};

/**
 * @brief Return statement.
 */
struct ReturnStatement : Tagged
{
    boost::optional<Expression> expr;
};

/**
 * @brief Function.
 */
struct Function
{
    std::string return_type;
    Identifier FuncName;
    std::list<Identifier> args;
    StatementList body;
};

/**
 * @brief Function list.
 */
using FunctionList = std::list<Function>;

/**
 * @brief Nil ostream operator.
 */
inline std::ostream& operator<<(std::ostream& out, Nil /*unused*/)
{
    out << "Nil";
    return out;
}

/**
 * @brief Identifier ostream operator.
 */
inline std::ostream& operator<<(std::ostream& out, Identifier const& id)
{
    out << id.Name;
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
    (tcc::ast::Identifier, FuncName)         //
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
    (tcc::ast::Identifier, FuncName)         //
    (std::list<tcc::ast::Identifier>, args)  //
    (tcc::ast::StatementList, body)          //
)

#endif
