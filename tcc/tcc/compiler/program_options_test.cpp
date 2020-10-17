/**
 * @file asm_utils_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcc/compiler/program_options.hpp"

#include "tcsl/tcsl.hpp"

#include "catch2/catch.hpp"

#include <boost/filesystem.hpp>

#include <vector>

using namespace std::string_literals;
using namespace Catch::Matchers;
namespace fs = boost::filesystem;

TEST_CASE("tcc/compiler: ProgramOptions", "[tcc][compiler]")
{
    SECTION("--help")
    {
        auto arguments   = std::vector<char const*> {"binary", "-h"};
        auto const argc  = static_cast<int>(arguments.size());
        auto const* argv = arguments.data();

        std::ostringstream stream;
        auto programOptions               = tcc::ProgramOptions {stream};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
        auto const cliOutput              = stream.str();

        CHECK(shouldExit);
        CHECK(exitCode == EXIT_SUCCESS);
        CHECK_THAT(cliOutput, Contains("Tobante's Crappy Compiler:"));
        CHECK_THAT(cliOutput, Contains("-O [ --optimization ] arg optimization level 0-1"));
    }

    SECTION("-O0")
    {
        auto arguments   = std::vector<char const*> {"binary", "-O0", "unkown_file.tcc"};
        auto const argc  = static_cast<int>(arguments.size());
        auto const* argv = arguments.data();

        auto stream                       = std::ostringstream();
        auto programOptions               = tcc::ProgramOptions {stream};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
        auto const cliOutput              = stream.str();

        // input file was not found
        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("Could not open input file: unkown_file.tcc\n"));

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

        auto stream                       = std::ostringstream();
        auto programOptions               = tcc::ProgramOptions {stream};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
        auto const cliOutput              = stream.str();

        // input file was not found
        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("Could not open input file: unkown_file.tcc\n"));

        auto compilerOptions = programOptions.GetCompilerOptions();
        CHECK(compilerOptions.OptLevel == 1);
        CHECK_FALSE(compilerOptions.PrintAssembly);
        CHECK_FALSE(compilerOptions.PrintAst);
        CHECK_FALSE(compilerOptions.PrintIR);
        CHECK_FALSE(compilerOptions.PrintSource);
    }

    SECTION("temp source file")
    {
        auto tempPath = fs::temp_directory_path() / fs::unique_path();
        auto tempFile = std::ofstream(tempPath.c_str());
        tempFile.close();

        auto arguments   = std::vector<char const*> {"binary", "-O1", tempPath.c_str()};
        auto const argc  = static_cast<int>(arguments.size());
        auto const* argv = arguments.data();

        auto programOptions               = tcc::ProgramOptions {};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);

        CHECK_FALSE(shouldExit);
        CHECK(exitCode == EXIT_SUCCESS);
    }

    SECTION("multiple source files not allowed currently")
    {
        auto const args = std::vector<char const*> {"binary", "unkown_file1.tcc", "unkown_file2.tcc"};
        auto const argc = static_cast<int>(args.size());
        auto const* argv = args.data();

        auto stream                       = std::ostringstream();
        auto programOptions               = tcc::ProgramOptions {stream};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
        auto const cliOutput              = stream.str();

        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("Only one source file allowed currently."));
    }

    SECTION("parse error")
    {
        auto const args  = std::vector<char const*> {"binary", "--heelp"};
        auto const argc  = static_cast<int>(args.size());
        auto const* argv = args.data();

        auto stream                       = std::ostringstream();
        auto programOptions               = tcc::ProgramOptions {stream};
        auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
        auto const cliOutput              = stream.str();

        CHECK(shouldExit);
        CHECK(exitCode == EXIT_FAILURE);
        CHECK_THAT(cliOutput, Contains("error: unrecognised option '--heelp'"));
    }
}