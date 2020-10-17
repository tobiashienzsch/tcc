/**
 * @file asm_utils_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/compiler/compiler.hpp"

#include "tcc/compiler/program_options.hpp"

#include "tcsl/tcsl.hpp"

#include "catch2/catch.hpp"

#include <vector>

using namespace std::string_literals;
using namespace Catch::Matchers;

TEST_CASE("tcc/compiler: ProgramOptions", "[tcc][compiler]")
{
    SECTION("-O0")
    {
        auto arguments   = std::vector<char const*> {"binary", "-O0", "unkown_file.tcc"};
        auto const argc  = static_cast<int>(arguments.size());
        auto const* argv = arguments.data();

        auto programOptions               = tcc::ProgramOptions {};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);

        // input file was not found
        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);

        auto compilerOptions = programOptions.GetCompilerOptions();
        CHECK(compilerOptions.OptLevel == 0);
        CHECK_FALSE(compilerOptions.PrintAssembly);
        CHECK_FALSE(compilerOptions.PrintAst);
        CHECK_FALSE(compilerOptions.PrintIR);
        CHECK_FALSE(compilerOptions.PrintSource);
    }

    SECTION("-O1")
    {
        auto arguments   = std::vector<char const*> {"binary", "-O1", "unkown_file.tcc"};
        auto const argc  = static_cast<int>(arguments.size());
        auto const* argv = arguments.data();

        auto programOptions               = tcc::ProgramOptions {};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);

        // input file was not found
        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);

        auto compilerOptions = programOptions.GetCompilerOptions();
        CHECK(compilerOptions.OptLevel == 1);
        CHECK_FALSE(compilerOptions.PrintAssembly);
        CHECK_FALSE(compilerOptions.PrintAst);
        CHECK_FALSE(compilerOptions.PrintIR);
        CHECK_FALSE(compilerOptions.PrintSource);
    }
}