/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/compiler/compiler.hpp"
#include "tcsl/tcsl.hpp"
#include "tcvm/vm/vm.hpp"

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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);

        REQUIRE(vm.Cpu() == 1);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 1"));
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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);

        REQUIRE(vm.Cpu() == 10);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 10"));
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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);

        REQUIRE(vm.Cpu() == 1);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 1"));
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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);

        REQUIRE(vm.Cpu() == 500);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 500"));
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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);

        REQUIRE(vm.Cpu() == 490);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 490"));
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

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);
        REQUIRE(vm.Cpu() == 60);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 60"));
    }
}

TEST_CASE("integration: CompileAndRunFCallMinimal", "[integration]")
{
    auto source = std::string {R"(
        int foo(a, b) { return a + b; }
        int main()
        {
            int x = 1;
            int y = 2;
            return foo(x, y);
        }
    )"};
    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);
        REQUIRE(vm.Cpu() == 3);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 3"));
    }
}

TEST_CASE("integration: CompileAndRunFCallNested", "[integration]")
{
    auto source = std::string {R"(
        int foo(a) { return a * 2; }
        int main()
        {
            int x = 1 + 4;
            int y = foo(foo(x * 2));
            return x + y; 
        }
    )"};
    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);
        REQUIRE(vm.Cpu() == 45);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 45"));
    }
}

TEST_CASE("integration: CompileAndRunFCallLocals", "[integration]")
{
    auto source = std::string {R"(
        int foo(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int main()
        {
            int x = 1 + 4;
            int y = foo(foo(x));
            return x + y;
        }
    )"};
    for (auto optLevel : {0, 1})
    {
        auto options     = tcc::CompilerOptions {};
        options.Source   = source;
        options.OptLevel = optLevel;

        auto compiler = tcc::Compiler {options};

        REQUIRE(compiler.Run() == EXIT_SUCCESS);

        auto const stackSize    = 200;
        auto const entryPoint   = compiler.GetEntryPoint();
        auto const globalMemory = 0;
        auto const assembly     = compiler.GetAssembly();
        auto stream             = std::stringstream {};
        auto vm = tcc::VirtualMachine(assembly, entryPoint, globalMemory,
                                      stackSize, true, stream);
        REQUIRE(vm.Cpu() == 310);
        REQUIRE_THAT(stream.str(), Catch::Contains("exit code: 310"));
    }
}
