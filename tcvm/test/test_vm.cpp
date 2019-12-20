/**
 * @file test_vm.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */
#include "catch2/catch.hpp"

#include "tcc/core/testing.hpp"

#include "tcvm/examples.hpp"
#include "tcvm/vm/byte_code.hpp"
#include "tcvm/vm/vm.hpp"

using tcc::ByteCode;
using tcc::Integer;
using tcc::TestCase;
using tcc::VirtualMachine;

TEST_CASE("vm: Halt", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer> {
        ByteCode::ICONST, 2,  //
        ByteCode::HALT,       //
    };

    auto vm             = VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == -1);
}

TEST_CASE("vm: Exit", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer> {
        ByteCode::ICONST, 2,  //
        ByteCode::EXIT,       //
    };

    auto vm             = VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 2);
}

TEST_CASE("vm: GlobalMemory", "[vm]")
{
    auto const assembly = std::vector<tcc::Integer> {
        ByteCode::ICONST, 143,  // push constant to stack
        ByteCode::GSTORE, 0,    // save to global
        ByteCode::ICONST, 2,    // do other stuff
        ByteCode::POP,          // ...
        ByteCode::GLOAD,  0,    // load from global
        ByteCode::EXIT,         // exit
    };

    auto const entryPoint = 0;
    auto const stackSize  = 50;
    auto const globalSize = 1;
    auto vm               = VirtualMachine(assembly, entryPoint, globalSize, stackSize, false);
    auto const exitCode   = vm.Cpu();

    REQUIRE(exitCode == 143);
}

TEST_CASE("vm: Addition", "[vm]")
{

    auto const testCases = {
        TestCase<Integer, Integer> {10, 40},  //
        TestCase<Integer, Integer> {20, 50},  //
    };

    for (auto const& test : testCases)
    {
        auto const assembly = tcvm::CreateAdditionAssembly(test.input);
        auto vm             = VirtualMachine(assembly, 18, 0, 200, false);
        auto const exitCode = vm.Cpu();
        REQUIRE(exitCode == test.expected);
    }
}

TEST_CASE("vm: Factorial", "[vm]")
{

    auto const testCases = {
        TestCase<Integer, Integer> {1, 1},    //
        TestCase<Integer, Integer> {3, 6},    //
        TestCase<Integer, Integer> {7, 5040}  //
    };

    for (auto const& test : testCases)
    {
        auto const factorial = tcvm::CreateFactorialAssembly(test.input);
        auto vm              = VirtualMachine(factorial, 22, 0, 200, false);
        auto const exitCode  = vm.Cpu();
        REQUIRE(exitCode == test.expected);
    }
}

TEST_CASE("vm: Fibonacci", "[vm]")
{
    auto const testCases = {
        TestCase<Integer, Integer> {1, 1},    //
        TestCase<Integer, Integer> {5, 5},    //
        TestCase<Integer, Integer> {12, 144}  //
    };

    for (auto const& test : testCases)
    {
        auto const assembly = tcvm::CreateFibonacciAssembly(test.input);
        auto vm             = VirtualMachine(assembly, 28, 0, 200, false);
        REQUIRE(vm.Cpu() == test.expected);
    }
}

TEST_CASE("vm: MultipleArguments", "[vm]")
{
    auto const assembly = tcvm::CreateMultipleArgumentsAssembly(10, 2);
    auto vm             = VirtualMachine(assembly, 6, 0, 100, false);
    auto const exitCode = vm.Cpu();

    REQUIRE(exitCode == 3);
}

TEST_CASE("vm: MultipleFunctions", "[vm]")
{
    auto const assembly = tcvm::CreateMultipleFunctionsAssembly(2);
    auto vm             = VirtualMachine(assembly, 12, 0, 200, false);
    auto const exitCode = vm.Cpu();
    REQUIRE(exitCode == 16);
}