#pragma once

#include <cstdint>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/compiler/lexer.hpp"

namespace tcc
{
enum class ExpressionType
{
    Number,
    Binary,
};

class SyntaxNode
{
public:
    virtual ~SyntaxNode()                  = default;
    virtual ExpressionType GetType() const = 0;
    virtual void Print() const             = 0;
};

class ExpressionSyntax : public SyntaxNode
{
};
class NumberExpressionSyntax : public ExpressionSyntax
{
public:
    NumberExpressionSyntax(SyntaxToken token) : m_token(token) {}
    ~NumberExpressionSyntax() override = default;
    ExpressionType GetType() const override { return ExpressionType::Number; }
    void Print() const override { std::cout << "NumberExpression\n"; }

private:
    SyntaxToken m_token;
};

class BinaryExpressionSyntax : public ExpressionSyntax
{
public:
    BinaryExpressionSyntax(std::shared_ptr<ExpressionSyntax const> left, std::shared_ptr<ExpressionSyntax const> right,
                           SyntaxToken const token)
        : m_left(left), m_right(right), m_operatorToken(token)
    {
    }

    ~BinaryExpressionSyntax() override = default;

    ExpressionType GetType() const override { return ExpressionType::Binary; }

    void Print() const override
    {
        std::cout << "Left: \n";
        m_left->Print();

        std::cout << m_operatorToken << '\n';

        std::cout << "Left: \n";
        m_left->Print();
    }

private:
    std::shared_ptr<ExpressionSyntax const> m_left;
    std::shared_ptr<ExpressionSyntax const> m_right;
    SyntaxToken const m_operatorToken;
};

class Parser
{
public:
    Parser(std::string_view const source)
    {
        auto lexer = tcc::Lexer(source);
        auto type  = tcc::SyntaxToken::Type{};

        do
        {
            auto const token = lexer.GetNextToken();
            type             = token.type;
            if (token.type != SyntaxToken::Type::WhiteSpace && token.type != SyntaxToken::Type::Unknown)
            {
                m_tokens.push_back(token);
            }

        } while (type != SyntaxToken::Type::EndOfFile);
    }

    std::shared_ptr<ExpressionSyntax> Parse()
    {
        auto const current = currentToken();
        auto left          = parsePrimaryExpression();
        left->Print();

        while (current.type == SyntaxToken::Type::Plus || current.type == SyntaxToken::Type::Minus)
        {
            auto const operatorToken = nextToken();
            auto const right         = parsePrimaryExpression();
            right->Print();

            left = std::make_shared<BinaryExpressionSyntax>(left, right, operatorToken);
        }

        return left;
    }

    tcc::SyntaxTokenList& GetTokens() { return m_tokens; }

private:
    std::shared_ptr<ExpressionSyntax> parsePrimaryExpression()
    {
        auto const numberToken = matchTokenType(SyntaxToken::Type::Number);
        return std::make_shared<NumberExpressionSyntax>(numberToken);
    }

    SyntaxToken matchTokenType(SyntaxToken::Type type)
    {
        auto const current = currentToken();
        if (current.type == type)
        {
            return nextToken();
        }

        return SyntaxToken{type, current.position, ""};
    }

    SyntaxToken nextToken()
    {
        auto const current = currentToken();
        m_position++;
        return current;
    }

    SyntaxToken peekToken(Integer offset)
    {
        auto const index = m_position + offset;
        if (index >= m_tokens.size())
        {
            return m_tokens.back();
        }

        return m_tokens.at(index);
    }

    SyntaxToken currentToken() { return peekToken(0); }

private:
    tcc::SyntaxTokenList m_tokens;
    UInteger m_position{0};
};

}  // namespace tcc