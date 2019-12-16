#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/parser.hpp"

int main(int const, char const** const)
{
    using namespace tcc;

    auto parser = tcc::Parser{"auto main(){}"};
    // auto parser = tcc::Parser{"auto 8(){}"};
    auto result = parser.Parse();

    for (auto& statement : result)
    {
        std::cout << statement << '\n';
    }

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
                std::make_unique<tcc::BinaryExpression>(std::make_unique<tcc::LiteralExpression>(4),  //
                                                        std::make_unique<tcc::LiteralExpression>(2),  //
                                                        tcc::BinaryExpression::Type::Add)             //
                )                                                                                     //
            )                                                                                         //
                                                                                                      //
    );

    // ICONST, 1    // 0
    // ICONST, 0    // 2
    // ICONST, 1    // 4
    // ICONST, 42   // 6

    // std::cout << statement << '\n';

    return EXIT_SUCCESS;
}