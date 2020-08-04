#include "tcc/cmd/program_options.hpp"
#include "tcc/compiler/compiler.hpp"

#include "tcsl/tcsl.hpp"

int main(int argc, char** argv)
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
