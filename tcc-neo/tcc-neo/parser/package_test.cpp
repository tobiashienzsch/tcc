/**
 * @file test_package.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc-neo/parser/package_def.hpp"

TEST_CASE("tcc-neo/parser: PackageDeclaration", "[parser]")
{
    auto [test_input, expected] = GENERATE(table<std::string, std::string>({
        {"package main;", "main"},            //
        {"package math;", "math"},            //
        {"package math_test;", "math_test"},  //
        {"package tobi_est;", "tobi_est"},    //
        {"package fmt8;", "fmt8"},            //
        {";", ""},                            //
    }));

    std::string result;
    tcc::parser::x3::phrase_parse(std::begin(test_input),     //
                                  std::end(test_input),       //
                                  tcc::PackageDeclaration(),  //
                                  tcc::parser::x3::space,     //
                                  result                      //
    );

    REQUIRE(result == expected);
}
