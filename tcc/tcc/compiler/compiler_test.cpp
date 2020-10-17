/**
 * @file asm_utils_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/compiler/compiler.hpp"

#include "tcsl/tcsl.hpp"

#include "catch2/catch.hpp"

using namespace std::string_literals;
using namespace Catch::Matchers;

TEST_CASE("tcc/compiler: Run", "[compiler]")
{
    SECTION("valid source input, no errors")
    {
        auto source  = std::string {"int main() { return 1 + 2; }"};
        auto stream  = std::ostringstream {};
        auto options = tcc::CompilerOptions {
            .Out      = &stream,
            .Source   = source,
            .OptLevel = 0,
        };

        auto compiler         = tcc::Compiler {options};
        auto const returnCode = compiler.Run();
        auto const cliOutput  = stream.str();

        CHECK(returnCode == EXIT_SUCCESS);
        CHECK(cliOutput.empty());
    }

    SECTION("valid source input, no errors, print.")
    {
        auto source  = std::string {"int main() { return 1 + 2; }"};
        auto stream  = std::ostringstream {};
        auto options = tcc::CompilerOptions {
            .Out           = &stream,
            .Source        = source,
            .OptLevel      = 0,
            .PrintSource   = true,
            .PrintIR       = true,
            .PrintAssembly = true,
        };

        auto compiler         = tcc::Compiler {options};
        auto const returnCode = compiler.Run();
        auto const cliOutput  = stream.str();

        CHECK(returnCode == EXIT_SUCCESS);
        // PrintSource
        CHECK_THAT(cliOutput, Contains("int main() { return 1 + 2; }"));
        // PrintIR
        CHECK_THAT(cliOutput, Contains("; program:"));
        // PrintAssembly
        CHECK_THAT(cliOutput, Contains("// asm: length="));
        // Ever valid program must have at least one each of the following instructions.
        CHECK_THAT(cliOutput, Contains("RET"));
        CHECK_THAT(cliOutput, Contains("EXIT"));
    }

    SECTION("invalid source input, parse errors")
    {
        auto source  = std::string {"int main() { return 1 + "};
        auto stream  = std::ostringstream {};
        auto options = tcc::CompilerOptions {
            .Out      = &stream,
            .Source   = source,
            .OptLevel = 0,
        };

        auto compiler         = tcc::Compiler {options};
        auto const returnCode = compiler.Run();
        auto const cliOutput  = stream.str();

        CHECK(returnCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("Error while parsing"));
    }

    SECTION("invalid source input, compile errors")
    {
        auto source  = std::string {"int main() { return a + b; }"};
        auto stream  = std::ostringstream {};
        auto options = tcc::CompilerOptions {
            .Out      = &stream,
            .Source   = source,
            .OptLevel = 0,
        };

        auto compiler         = tcc::Compiler {options};
        auto const returnCode = compiler.Run();
        auto const cliOutput  = stream.str();

        CHECK(returnCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("Error while compiling"));
        CHECK_THAT(cliOutput, Contains("Undeclared variable: a line 1:"));
    }
}