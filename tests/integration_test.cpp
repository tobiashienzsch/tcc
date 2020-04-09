/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/compiler/compiler.hpp"
#include "tcvm/vm/vm.hpp"
#include "tsl/tsl.hpp"

#include "catch2/catch.hpp"

TEST_CASE("integration: CompileAndRunConstant", "[integration]")
{
    auto source = std::string {R"(
        int main() { return 1;}
    )"};

    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();

        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 1);
    }
}

TEST_CASE("integration: CompileAndRunAddition", "[integration]")
{
    auto source = std::string {R"(
        int main() { return 1+2+3+4;}
    )"};

    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto vm                 = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 10);
    }
}

TEST_CASE("integration: CompileAndRunSubtraction", "[integration]")
{
    auto source = std::string {R"(
        int main() { return 30-20-9;}
    )"};

    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto vm                 = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 1);
    }
}

TEST_CASE("integration: CompileAndRunMultiplication", "[integration]")
{
    auto source = std::string {R"(
        int main() { return 10*10*5;}
    )"};

    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto vm                 = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 500);
    }
}

TEST_CASE("integration: CompileAndRunMixedExpression", "[integration]")
{
    auto source = std::string {R"(
        int main() { return 10*10*5+10-20;}
    )"};

    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto vm                 = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 490);
    }
}

TEST_CASE("integration: CompileAndRunLocalVars", "[integration]")
{
    auto source = std::string {R"(
        int main() { 
            int x = (1+2+3+4)*2;
            int y = 10*5-10;
            return x+y;
        }
    )"};
    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;
        auto compiler    = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = 0;
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();

        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory, stackSize, false);
        REQUIRE(vm.Cpu() == 60);
    }
}
