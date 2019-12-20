/**
 * @file test_lexer.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler_old/ast.hpp"

TEST_CASE("ast: LiteralExpression", "[ast]")
{
    auto const val        = 5;
    auto const expression = tcc::LiteralExpression(val);
    auto const assembly   = expression.GetAssembly();
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == val);
}

TEST_CASE("ast: BinaryExpression", "[ast]")
{
    auto const expression = tcc::BinaryExpression(std::make_unique<tcc::LiteralExpression>(4),  //
                                                  std::make_unique<tcc::LiteralExpression>(2),  //
                                                  tcc::BinaryExpression::Type::Add              //
    );

    auto const assembly = expression.GetAssembly();
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 4);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(3) == 2);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(4)) == tcc::ByteCode::IADD);
}

TEST_CASE("ast: TenerayExpression", "[ast]")
{
    auto expression = tcc::TenerayExpression(std::make_unique<tcc::LiteralExpression>(true),  // condition
                                             std::make_unique<tcc::LiteralExpression>(42),    // true case
                                             std::make_unique<tcc::LiteralExpression>(2)      // false case
    );

    // true ? 42 : 2
    // .condition
    // ICONST 1         // 0
    // BRT 7            // 2

    // .false case
    // ICONST 2         // 4

    // .true case
    // ICONST 42        // 6

    auto const assembly = expression.GetAssembly();
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 1);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::BRT);
    REQUIRE(assembly.at(3) == 6);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(4)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(5) == 2);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(6)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(7) == 42);
}

TEST_CASE("ast: ExpressionStatement", "[ast]")
{
    auto statement      = tcc::ExpressionStatement(std::make_unique<tcc::LiteralExpression>(true));
    auto const assembly = statement.GetAssembly();

    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 1);
}

TEST_CASE("ast: ReturnStatement", "[ast]")
{
    auto statement      = tcc::ReturnStatement(std::make_unique<tcc::LiteralExpression>(true));
    auto const assembly = statement.GetAssembly();

    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 1);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::RET);
}

TEST_CASE("ast: ConditionalStatement", "[ast]")
{
    auto statement      = tcc::ConditionalStatement(             //
        std::make_unique<tcc::LiteralExpression>(true),     // condition
        std::make_unique<tcc::ExpressionStatement>(         // true case
            std::make_unique<tcc::LiteralExpression>(true)  //
            )                                               //
    );
    auto const assembly = statement.GetAssembly();

    // ICONST, 1    // 0 condition
    // BRF, 99      // 2 branch if false
    // ICONST, 1    // 4 true case
    // ICONST, 2    // 6 false case

    REQUIRE(assembly.size() == 6);

    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 1);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::BRF);
    REQUIRE(assembly.at(3) == 6);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(4)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(5) == 1);
}

TEST_CASE("ast: CompoundStatement", "[ast]")
{
    auto statement = tcc::CompoundStatement(                 //
        std::make_unique<tcc::ExpressionStatement>(          // first statement
            std::make_unique<tcc::LiteralExpression>(true)   //
            ),                                               //
        std::make_unique<tcc::ExpressionStatement>(          // second statement
            std::make_unique<tcc::LiteralExpression>(false)  //
            )                                                //
    );

    auto const assembly = statement.GetAssembly(0);

    // ICONST, 1    // 0 condition
    // ICONST, 0    // 2 condition

    REQUIRE(assembly.size() == 4);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(0)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(1) == 1);
    REQUIRE(static_cast<tcc::ByteCode>(assembly.at(2)) == tcc::ByteCode::ICONST);
    REQUIRE(assembly.at(3) == 0);
}

TEST_CASE("ast: CompoundStatementNested", "[ast]")
{
    auto statement = tcc::CompoundStatement(                    //
        std::make_unique<tcc::ExpressionStatement>(             // first statement
            std::make_unique<tcc::LiteralExpression>(true)      //
            ),                                                  //
        std::make_unique<tcc::ExpressionStatement>(             // second statement
            std::make_unique<tcc::LiteralExpression>(false)     //
            ),                                                  //
        std::make_unique<tcc::CompoundStatement>(               // nested compund statement
            std::make_unique<tcc::ExpressionStatement>(         //     first statement
                std::make_unique<tcc::LiteralExpression>(true)  //
                ),                                              //
            std::make_unique<tcc::ExpressionStatement>(         //     second statement
                std::make_unique<tcc::LiteralExpression>(42)    //
                )                                               //
            )                                                   //
    );

    // ICONST, 1    // 0
    // ICONST, 0    // 2
    // ICONST, 1    // 4
    // ICONST, 42   // 6

    auto const assembly = statement.GetAssembly(0);

    REQUIRE(assembly.size() == 8);
    REQUIRE(assembly.at(7) == 42);
}