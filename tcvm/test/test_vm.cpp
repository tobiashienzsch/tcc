/**
 * @file test_vm.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcvm/examples.hpp"
#include "tcvm/vm/byte_code.hpp"
#include "tcvm/vm/vm.hpp"

using tcc::ByteCode;

namespace
{
struct TestCase
{
    tcc::Integer input;
    tcc::Integer expected;
};

}  // namespace
TEST_CASE("vm: Halt", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer>{
        ByteCode::ICONST, 2,  //
        ByteCode::HALT,       //
    };

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == -1);
}

TEST_CASE("vm: Exit", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer>{
        ByteCode::ICONST, 2,  //
        ByteCode::EXIT,       //
    };

    auto vm             = tcc::VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 2);
}

TEST_CASE("vm: GlobalMemory", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer>{
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

TEST_CASE("vm: Factorial", "[vm]")
{

    auto const testCases = {
        TestCase{1, 1},    //
        TestCase{3, 6},    //
        TestCase{7, 5040}  //
    };

    for (auto const& test : testCases)
    {
        auto const factorial = tcvm::createFactorialAssembly(test.input);
        auto vm              = tcc::VirtualMachine(factorial, 22, 0, 200, false);
        auto const exitCode  = vm.Cpu();
        REQUIRE(exitCode == test.expected);
    }
}

TEST_CASE("vm: Fibonacci", "[vm]")
{
    auto const testCases = {
        TestCase{1, 1},    //
        TestCase{5, 5},    //
        TestCase{12, 144}  //
    };

    for (auto const& test : testCases)
    {
        auto const assembly = tcvm::CreateFibonacciAssembly(test.input);
        auto vm             = tcc::VirtualMachine(assembly, 28, 0, 200, false);
        REQUIRE(vm.Cpu() == test.expected);
    }
}

TEST_CASE("vm: MultipleArguments", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer>{
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
    auto const assembly = std::vector<tcc::Integer>{
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