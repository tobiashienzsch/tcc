#include "tcc/asm/assembly_generator.hpp"
#include "tcc/cmd/program_options.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/parser.hpp"
#include "tsl/tsl.hpp"

int main(int argc, char** argv)
{
    auto programOptions               = tcc::ProgramOptions {};
    auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
    if (shouldExit)
    {
        return exitCode;
    }

    auto const& flags = programOptions.GetCompilerFlags();
    auto iter         = flags.Source.cbegin();
    auto end          = flags.Source.cend();

    using IteratorType = std::string::const_iterator;
    auto errorHandler  = tcc::ErrorHandler<IteratorType> {iter, end, std::cerr};

    auto parser = tcc::Parser {errorHandler};
    if (!parser.ParseSource(iter, end))
    {
        fmt::print("Error while parsing!\n");
        return EXIT_FAILURE;
    }

    auto irGenerator = tcc::IRGenerator {errorHandler};  // IR Generator
    if (!irGenerator(parser.GetAst()))
    {
        fmt::print("Error while compiling!\n");
        return EXIT_FAILURE;
    }

    auto& currentFunction = irGenerator.CurrentPackage().functions[0];

    if (flags.OptLevel > 0)
    {
        auto optimizer = tcc::Optimizer(currentFunction);
        optimizer.Optimize();
    }

    if (flags.PrintSource)
    {
        fmt::print("source:\n{}\n", flags.Source);
    }

    if (flags.PrintIR)
    {
        fmt::print("{}", irGenerator.CurrentPackage());
    }

    if (!flags.OutputName.empty())
    {
        auto assembly      = tcc::AssemblyGenerator::Build(currentFunction);
        auto binaryProgram = tcc::BinaryProgram {1, flags.OutputName, 0, assembly};
        if (!tcc::BinaryFormat::WriteToFile(flags.OutputName, binaryProgram))
        {
            fmt::print("Error while writing binary!\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
