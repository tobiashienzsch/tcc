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
    int id {};
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
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Identifier(std::string n = "") : name(std::move(n)) { }
    std::string name;
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
    Operand operand;
};

/**
 * @brief Operation.
 */
struct Operation
{
    OpToken Operator {OpToken::Invalid};
    Operand operand;
};

/**
 * @brief Function call.
 */
struct FunctionCall
{
    Identifier funcName;
    std::vector<Expression> args;
};

/**
 * @brief Expression.
 */
struct Expression
{
    Operand first;
    std::vector<Operation> rest;
};

/**
 * @brief Assignment.
 */
struct Assignment
{
    Identifier left;
    Expression right;
};

/**
 * @brief Variable Declaration.
 */
struct VariableDeclaration
{
    Identifier left;
    boost::optional<Expression> right;
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
    Expression condition;
    Statement then;
    boost::optional<Statement> Else;
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
    std::string returnType;
    Identifier funcName;
    std::vector<Identifier> args;
    StatementList body;
};

/**
 * @brief Function list.
 */
using FunctionList = std::vector<Function>;

/**
 * @brief Nil ostream operator.
 */
inline auto operator<<(std::ostream& out, Nil /*unused*/) -> std::ostream&
{
    out << "Nil";
    return out;
}

/**
 * @brief Identifier ostream operator.
 */
inline auto operator<<(std::ostream& out, Identifier const& id) -> std::ostream&
{
    out << id.name;
    return out;
}
}  // namespace ast
}  // namespace tcc

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Unary,               //
    (tcc::ast::OpToken, Operator)  //
    (tcc::ast::Operand, operand)   //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Operation,           //
    (tcc::ast::OpToken, Operator)  //
    (tcc::ast::Operand, operand)   //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                     //
    tcc::ast::FunctionCall,                    //
    (tcc::ast::Identifier, funcName)           //
    (std::vector<tcc::ast::Expression>, args)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                    //
    tcc::ast::Expression,                     //
    (tcc::ast::Operand, first)                //
    (std::vector<tcc::ast::Operation>, rest)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                          //
    tcc::ast::VariableDeclaration,                  //
    (tcc::ast::Identifier, left)                    //
    (boost::optional<tcc::ast::Expression>, right)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(         //
    tcc::ast::Assignment,          //
    (tcc::ast::Identifier, left)   //
    (tcc::ast::Expression, right)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                        //
    tcc::ast::IfStatement,                        //
    (tcc::ast::Expression, condition)             //
    (tcc::ast::Statement, then)                   //
    (boost::optional<tcc::ast::Statement>, Else)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(             //
    tcc::ast::WhileStatement,          //
    (tcc::ast::Expression, condition)  //
    (tcc::ast::Statement, body)        //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                         //
    tcc::ast::ReturnStatement,                     //
    (boost::optional<tcc::ast::Expression>, expr)  //
)

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
BOOST_FUSION_ADAPT_STRUCT(                     //
    tcc::ast::Function,                        //
    (std::string, returnType)                  //
    (tcc::ast::Identifier, funcName)           //
    (std::vector<tcc::ast::Identifier>, args)  //
    (tcc::ast::StatementList, body)            //
)

#endif
