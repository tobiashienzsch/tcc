#include "tcc/compiler/compiler.hpp"
#include "tcc/compiler/program_options.hpp"

#include "tcsl/tcsl.hpp"

auto main(int argc, char const* const* argv) -> int
{
    auto programOptions               = tcc::ProgramOptions {};
    auto const [shouldExit, exitCode] = programOptions.parseArguments(argc, argv);
    if (shouldExit) { return exitCode; }

    auto compiler = tcc::Compiler {programOptions.getCompilerOptions()};
    return compiler.run();
}
