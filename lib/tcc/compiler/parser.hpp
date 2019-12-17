#pragma once

#include <cstdint>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/compiler/ast.hpp"
#include "tcc/compiler/lexer.hpp"

namespace tcc
{
class Parser
{
public:
    Parser(std::string_view const source)
    {
        auto lexer = tcc::Lexer(source);
        auto type  = tcc::SyntaxToken::Type {};

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

    tcc::SyntaxTokenList& GetTokens() { return m_tokens; }

    std::vector<Statement::Ptr> Parse()
    {
        auto result = std::vector<Statement::Ptr> {};

        while (m_position < m_tokens.size())
        {
            auto const current = currentToken();
            switch (current.type)
            {
                case SyntaxToken::Type::Auto:
                {
                    // auto must be followed by an identifier
                    nextToken();
                    if (currentToken().type != SyntaxToken::Type::Identifier)
                    {
                        std::cerr << "Expected identifier after keyword: " << current.type << '\n';
                        return result;
                    }

                    // var declaration
                    nextToken();
                    if (currentToken().type == SyntaxToken::Type::Equal)
                    {
                        return result;
                    }

                    // function declaration
                    if (currentToken().type == SyntaxToken::Type::OpenBrace)
                    {
                        return result;
                    }

                    break;
                }

                case SyntaxToken::Type::Number:
                {
                    std::stringstream numberStr(std::string(current.text));
                    int val = 0;
                    numberStr >> val;

                    // addition
                    if (peekToken(2).type == SyntaxToken::Type::Plus)
                    {
                        result.push_back(std::make_unique<tcc::ExpressionStatement>(  // first statement
                            std::make_unique<tcc::LiteralExpression>(val)             //
                            ));
                    }

                    // function declaration
                    if (peekToken(2).type == SyntaxToken::Type::OpenBrace)
                    {
                        return result;
                    }

                    break;
                }

                default:
                {
                    std::cerr << "Unknown token type with id: " << current.type << '\n';
                    break;
                }
            }

            auto _ = nextToken();
        }

        return {};
    }

private:
    SyntaxToken matchTokenType(SyntaxToken::Type type)
    {
        auto const current = currentToken();
        if (current.type == type)
        {
            return nextToken();
        }

        return SyntaxToken {type, current.position, ""};
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
    UInteger m_position {0};
};

}  // namespace tcc