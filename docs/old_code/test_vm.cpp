/**
 * @file test_vm.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */
#include "catch2/catch.hpp"

#include "tcc/compiler_old/ast.hpp"
#include "tcc/core.hpp"
#include "tcc/vm/vm.hpp"

using tcc::ByteCode;

TEST_CASE("vm: Halt", "[vm]")
{
    auto const assembly = std::vector<int64_t>{
        ByteCode::ICONST, 2,  //
        ByteCode::HALT,       //
    };

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == -1);
}

TEST_CASE("vm: Exit", "[vm]")
{
    auto const assembly = std::vector<int64_t>{
        ByteCode::ICONST, 2,  //
        ByteCode::EXIT,       //
    };

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 2);
}

TEST_CASE("vm: GlobalMemory", "[vm]")
{
    auto const assembly = std::vector<int64_t>{
        ByteCode::ICONST, 143,  // push constant to stack
        ByteCode::GSTORE, 0,    // save to global
        ByteCode::ICONST, 2,    // do other stuff
        ByteCode::PRINT,        // ...
        ByteCode::GLOAD,  0,    // load from global
        ByteCode::EXIT,         // exit
    };

    auto const entryPoint = 0;
    auto const stackSize  = 50;
    auto const globalSize = 1;
    auto vm               = tcc::VirtualMachine(assembly, entryPoint, globalSize, stackSize, false);
    auto const exitCode   = vm.Cpu();

    REQUIRE(exitCode == 143);
}

TEST_CASE("vm: Simple Expression", "[vm]")
{
    auto const expression = tcc::BinaryExpression(std::make_unique<tcc::LiteralExpression>(3),  //
                                                  std::make_unique<tcc::LiteralExpression>(2),  //
                                                  tcc::BinaryExpression::Type::Subtract         //
    );
    auto assembly         = expression.GetAssembly();
    assembly.push_back(ByteCode::EXIT);

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 100, false);
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

    auto vm             = tcc::VirtualMachine(factorial, 22, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 6);
}

TEST_CASE("vm: Fibonacci", "[vm]")
{
    auto const createFibonacciAssembly = [](int64_t const arg) {
        return std::vector<int64_t>{
            // .def fib: args=1, locals=0
            // if (x < 2) return x;
            ByteCode::LOAD, -3,   // 0
            ByteCode::ICONST, 2,  // 2
            ByteCode::ILT,        // 4
            ByteCode::BRF, 10,    // 5
            ByteCode::LOAD, -3,   // 7
            ByteCode::RET,        // 9

            // return fib(x - 1) + fib(x - 2)
            ByteCode::LOAD, -3,    // 10
            ByteCode::ICONST, 1,   // 12
            ByteCode::ISUB,        // 14
            ByteCode::CALL, 0, 1,  // 15 <-- fib(x-1)
            ByteCode::LOAD, -3,    // 18
            ByteCode::ICONST, 2,   // 20
            ByteCode::ISUB,        // 22
            ByteCode::CALL, 0, 1,  // 23 <-- fib(x-2)
            ByteCode::IADD,        // 26
            ByteCode::RET,         // 27

            // .def main: args=0, locals=0
            // return fib(arg);
            ByteCode::ICONST, arg,  // 28 <-- MAIN
            ByteCode::CALL, 0, 1,   // 30 <-- fib(arg)
            ByteCode::EXIT,         // 33
        };
    };

    SECTION("1")
    {
        auto const assembly = createFibonacciAssembly(1);
        auto vm             = tcc::VirtualMachine(assembly, 28, 0, 200, false);
        REQUIRE(vm.Cpu() == 1);
    }

    SECTION("5")
    {
        auto const assembly = createFibonacciAssembly(5);
        auto vm             = tcc::VirtualMachine(assembly, 28, 0, 200, false);
        REQUIRE(vm.Cpu() == 5);
    }

    SECTION("12")
    {
        auto const assembly = createFibonacciAssembly(12);
        auto vm             = tcc::VirtualMachine(assembly, 28, 0, 200, false);
        REQUIRE(vm.Cpu() == 144);
    }
}

TEST_CASE("vm: MultipleArguments", "[vm]")
{
    auto const assembly = std::vector<int64_t>{
        // .def sub: args=2, locals=0
        // return x + y;
        ByteCode::LOAD, -4,  // 0 <-- load x
        ByteCode::LOAD, -3,  // 2 <-- load y
        ByteCode::ISUB,      // 4
        ByteCode::RET,       // 5

        // .def main: args=0, locals=0
        // x = sub(10, 2);
        // y = sub(x, 5);
        // exit x;
        ByteCode::ICONST, 10,  // 6 <-- MAIN
        ByteCode::ICONST, 2,   // 8
        ByteCode::CALL, 0, 2,  // 10 <-- sub(10, 2)
        ByteCode::ICONST, 5,   // 13
        ByteCode::CALL, 0, 2,  // 13 <-- sub(x, 5)
        ByteCode::EXIT,        // 13
    };

    auto vm             = tcc::VirtualMachine(assembly, 6, 0, 100, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 3);
}

TEST_CASE("vm: MultipleFunctions", "[vm]")
{
    auto const assembly = std::vector<int64_t>{
        // .def func1: args=1, locals=0
        // return x * 3;
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 3,  // 2
        ByteCode::IMUL,       // 4
        ByteCode::RET,        // 5

        // .def func2: args=1, locals=0
        // return x + 10;
        ByteCode::LOAD, -3,    // 6
        ByteCode::ICONST, 10,  // 8
        ByteCode::IADD,        // 10
        ByteCode::RET,         // 11

        // .def main: args=0, locals=0
        // x = func1(2);
        // y = func2(x);
        // exit y;
        ByteCode::ICONST, 2,   // 12 <-- MAIN
        ByteCode::CALL, 0, 1,  // 14 <-- func1(2)
        ByteCode::CALL, 6, 1,  // 17 <-- func2(func1(2))
        ByteCode::EXIT,        // 21
    };

    auto vm             = tcc::VirtualMachine(assembly, 12, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 16);
}