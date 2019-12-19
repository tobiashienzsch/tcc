#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler_old/ast.hpp"

int main(int const, char const** const)
{
    using namespace tcc;

    auto statement =                                                                                      //
        std::make_unique<tcc::FunctionStatement>(                                                         //
            "main",                                                                                       //
            std::make_unique<tcc::CompoundStatement>(                                                     //
                std::make_unique<tcc::AssignmentStatement>(                                               //
                    "x",                                                                                  //
                    std::make_unique<tcc::BinaryExpression>(                                              //
                        std::make_unique<tcc::LiteralExpression>(4),                                      //
                        std::make_unique<tcc::BinaryExpression>(                                          //
                            std::make_unique<tcc::LiteralExpression>(4),                                  //
                            std::make_unique<tcc::LiteralExpression>(2),                                  //
                            tcc::BinaryExpression::Type::Add),                                            //
                        tcc::BinaryExpression::Type::Add),                                                //
                    true                                                                                  //
                    ),                                                                                    //
                std::make_unique<tcc::ExpressionStatement>(                                               //
                    std::make_unique<tcc::BinaryExpression>(std::make_unique<tcc::LiteralExpression>(4),  //
                                                            std::make_unique<tcc::LiteralExpression>(2),  //
                                                            tcc::BinaryExpression::Type::Add)             //
                    ),                                                                                    //
                std::make_unique<tcc::ReturnStatement>(                                                   //
                    std::make_unique<tcc::VariableExpression>("x")                                        //
                    )                                                                                     //
                )                                                                                         //
        );

    std::cout << statement.get() << '\n';

    return EXIT_SUCCESS;
}