#if !defined(TCC_PARSER_QI_AST_HPP)
#define TCC_PARSER_QI_AST_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <utility>
#include <vector>

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
    OpToken Operator {OpToken::Invalid};
    Operand Operand_;
};

/**
 * @brief Operation.
 */
struct Operation
{
    OpToken Operator {OpToken::Invalid};
    Operand Operand_;
};

/**
 * @brief Function call.
 */
struct FunctionCall
{
    Identifier FuncName;
    std::vector<Expression> Args;
};

/**
 * @brief Expression.
 */
struct Expression
{
    Operand First;
    std::vector<Operation> Rest;
};

/**
 * @brief Assignment.
 */
struct Assignment
{
    Identifier Left;
    Expression Right;
};

/**
 * @brief Variable Declaration.
 */
struct VariableDeclaration
{
    Identifier Left;
    boost::optional<Expression> Right;
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
struct StatementList : std::vector<Statement>
{
};

/**
 * @brief If statement.
 */
struct IfStatement
{
    Expression Condition;
    Statement Then;
    boost::optional<Statement> Else;
};

/**
 * @brief While statement.
 */
struct WhileStatement
{
    Expression Condition;
    Statement Body;
};

/**
 * @brief Return statement.
 */
struct ReturnStatement : Tagged
{
    boost::optional<Expression> Expr;
};

/**
 * @brief Function.
 */
struct Function
{
    std::string ReturnType;
    Identifier FuncName;
    std::vector<Identifier> Args;
    StatementList Body;
};

/**
 * @brief Function list.
 */
using FunctionList = std::vector<Function>;

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

BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Unary,               //
    (tcc::ast::OpToken, Operator)  //
    (tcc::ast::Operand, Operand_)  //
)

BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Operation,           //
    (tcc::ast::OpToken, Operator)  //
    (tcc::ast::Operand, Operand_)  //
)

BOOST_FUSION_ADAPT_STRUCT(                     //
    tcc::ast::FunctionCall,                    //
    (tcc::ast::Identifier, FuncName)           //
    (std::vector<tcc::ast::Expression>, Args)  //
)

BOOST_FUSION_ADAPT_STRUCT(                    //
    tcc::ast::Expression,                     //
    (tcc::ast::Operand, First)                //
    (std::vector<tcc::ast::Operation>, Rest)  //
)

BOOST_FUSION_ADAPT_STRUCT(                          //
    tcc::ast::VariableDeclaration,                  //
    (tcc::ast::Identifier, Left)                    //
    (boost::optional<tcc::ast::Expression>, Right)  //
)

BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Assignment,          //
    (tcc::ast::Identifier, Left)   //
    (tcc::ast::Expression, Right)  //
)

BOOST_FUSION_ADAPT_STRUCT(                        //
    tcc::ast::IfStatement,                        //
    (tcc::ast::Expression, Condition)             //
    (tcc::ast::Statement, Then)                   //
    (boost::optional<tcc::ast::Statement>, Else)  //
)

BOOST_FUSION_ADAPT_STRUCT(             //
    tcc::ast::WhileStatement,          //
    (tcc::ast::Expression, Condition)  //
    (tcc::ast::Statement, Body)        //
)

BOOST_FUSION_ADAPT_STRUCT(                         //
    tcc::ast::ReturnStatement,                     //
    (boost::optional<tcc::ast::Expression>, Expr)  //
)

BOOST_FUSION_ADAPT_STRUCT(                     //
    tcc::ast::Function,                        //
    (std::string, ReturnType)                  //
    (tcc::ast::Identifier, FuncName)           //
    (std::vector<tcc::ast::Identifier>, Args)  //
    (tcc::ast::StatementList, Body)            //
)

#endif
