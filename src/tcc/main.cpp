#include "tcc/compiler/compiler.hpp"
#include "tcc/compiler/program_options.hpp"

#include "tcsl/tcsl.hpp"

int main(int argc, char const* const* argv)
{
    auto programOptions               = tcc::ProgramOptions {};
    auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
    if (shouldExit)
    {
        return exitCode;
    }

    auto compiler = tcc::Compiler {programOptions.GetCompilerOptions()};
    return compiler.Run();
}
