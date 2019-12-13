/**
 * @file test_vm.cpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler/ast.hpp"
#include "tcc/vm/byte_code.hpp"
#include "tcc/vm/vm.hpp"

using tcc::ByteCode;

TEST_CASE("vm: Simple Expression", "[vm]")
{
    tcc::BinaryExpression expression = {
        new tcc::ConstantExpression(3),        //
        new tcc::ConstantExpression(2),        //
        tcc::BinaryExpression::Type::Subtract  //
    };
    auto assembly = expression.GetAssembly();
    assembly.push_back(ByteCode::EXIT);

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 100);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 1);
}

TEST_CASE("vm: Factorial", "[vm]")
{
    auto const argument  = 3;
    auto const factorial = std::vector<int64_t>{
        // .def fact: args=1, locals=0
        // if n < 2 return 1
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 2,  // 2
        ByteCode::ILT,        // 4
        ByteCode::BRF, 10,    // 5
        ByteCode::ICONST, 1,  // 7
        ByteCode::RET,        // 9
        // return n * fact(n-1)
        ByteCode::LOAD, -3,    // 10
        ByteCode::LOAD, -3,    // 12
        ByteCode::ICONST, 1,   // 14
        ByteCode::ISUB,        // 16
        ByteCode::CALL, 0, 1,  // 17
        ByteCode::IMUL,        // 20
        ByteCode::RET,         // 21

        // .def main: args=0, locals=0
        ByteCode::ICONST, argument,  // 22 <-- MAIN
        ByteCode::CALL, 0, 1,        // 24
        ByteCode::EXIT,              // 27
    };

    auto vm             = tcc::VirtualMachine(factorial, 22, 0, 200);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 6);
}