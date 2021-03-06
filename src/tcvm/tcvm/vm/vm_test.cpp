/**
 * @file vm_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcvm/vm/vm.hpp"

#include "catch2/catch.hpp"
#include "tcsl/tcsl.hpp"
#include "tcvm/examples.hpp"

using tcc::ByteCode;
using tcc::TestCase;
using tcc::VirtualMachine;

TEST_CASE("tcvm: Halt", "[tcvm]")
{
    auto const assembly = std::vector<int64_t> {
        ByteCode::ICONST, 2,  //
        ByteCode::HALT,       //
    };

    auto vm             = VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.cpu();

    REQUIRE(exitCode == -1);
}

TEST_CASE("tcvm: Exit", "[tcvm]")
{
    auto const assembly = std::vector<int64_t> {
        ByteCode::ICONST, 2,  //
        ByteCode::EXIT,       //
    };

    auto vm             = VirtualMachine(assembly, 0, 0, 200, false);
    auto const exitCode = vm.cpu();

    REQUIRE(exitCode == 2);
}

TEST_CASE("tcvm: GlobalMemory", "[tcvm]")
{
    auto const assembly = std::vector<int64_t> {
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
    auto const exitCode   = vm.cpu();

    REQUIRE(exitCode == 143);
}

TEST_CASE("tcvm: Addition", "[tcvm]")
{
    auto const testCases = {
        TestCase<int64_t, int64_t> {10, 40},  //
        TestCase<int64_t, int64_t> {20, 50},  //
    };

    for (auto const& test : testCases)
    {
        auto const assembly = tcvm::createAdditionProgram(test.input);
        auto vm             = VirtualMachine(assembly.data, assembly.entryPoint, 0, 200, false);
        auto const exitCode = vm.cpu();
        REQUIRE(exitCode == test.expected);
    }
}

TEST_CASE("tcvm: Factorial", "[tcvm]")
{
    auto const testCases = {
        TestCase<int64_t, int64_t> {1, 1},    //
        TestCase<int64_t, int64_t> {3, 6},    //
        TestCase<int64_t, int64_t> {7, 5040}  //
    };

    for (auto const& test : testCases)
    {
        auto const factorial = tcvm::createFactorialProgram(test.input);
        auto vm              = VirtualMachine(factorial.data, factorial.entryPoint, 0, 200, false);
        auto const exitCode  = vm.cpu();
        REQUIRE(exitCode == test.expected);
    }
}

TEST_CASE("tcvm: Fibonacci", "[tcvm]")
{
    auto const testCases = {
        TestCase<int64_t, int64_t> {1, 1},    //
        TestCase<int64_t, int64_t> {5, 5},    //
        TestCase<int64_t, int64_t> {12, 144}  //
    };

    for (auto const& test : testCases)
    {
        auto const assembly = tcvm::createFibonacciProgram(test.input);
        auto vm             = VirtualMachine(assembly.data, assembly.entryPoint, 0, 200, false);
        REQUIRE(vm.cpu() == test.expected);
    }
}

TEST_CASE("tcvm: MultipleArguments", "[tcvm]")
{
    auto const assembly = tcvm::createMultipleArgumentsProgram(10, 2);
    auto vm             = VirtualMachine(assembly.data, assembly.entryPoint, 0, 100, false);
    auto const exitCode = vm.cpu();

    REQUIRE(exitCode == 3);
}

TEST_CASE("tcvm: MultipleFunctions", "[tcvm]")
{
    auto const assembly = tcvm::createMultipleFunctionsProgram(2);
    auto vm             = VirtualMachine(assembly.data, assembly.entryPoint, 0, 200, false);
    auto const exitCode = vm.cpu();
    REQUIRE(exitCode == 16);
}