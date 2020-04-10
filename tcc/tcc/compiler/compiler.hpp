#if !defined(TCC_COMPILER_COMPILER_HPP)
#define TCC_COMPILER_COMPILER_HPP

#include "tcc/asm/assembly_generator.hpp"
#include "tcc/compiler/options.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/parser.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
class Compiler
{
public:
    explicit Compiler(CompilerOptions opt) : options_(std::move(opt)) { }

    [[nodiscard]] int Run()
    {
        auto iter = options_.Source.cbegin();
        auto end  = options_.Source.cend();

        using IteratorType = std::string::const_iterator;
        auto errorHandler
            = tcc::ErrorHandler<IteratorType> {iter, end, std::cerr};

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

        if (options_.OptLevel > 0)
        {
            for (auto& func : irGenerator.CurrentPackage().functions)
            {
                auto optimizer = tcc::Optimizer(func);
                optimizer.Optimize();
            }
        }

        if (options_.PrintSource)
        {
            fmt::print("source:\n{}\n", options_.Source);
        }

        if (options_.PrintIR)
        {
            fmt::print("{}", irGenerator.CurrentPackage());
        }

        auto const& package = irGenerator.CurrentPackage();
        assembly_           = tcc::AssemblyGenerator::Build(package);

        if (options_.PrintAssembly)
        {
            tcc::AssemblyGenerator::Print(assembly_.first);
        }

        if (!options_.OutputName.empty())
        {
            auto binaryProgram = tcc::BinaryProgram {
                1, options_.OutputName, assembly_.second, assembly_.first};
            if (!tcc::BinaryFormat::WriteToFile(options_.OutputName,
                                                binaryProgram))
            {
                fmt::print("Error while writing binary!\n");
                return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }

    [[nodiscard]] std::vector<int64_t> const& GetAssembly() const
    {
        return assembly_.first;
    }
    [[nodiscard]] int64_t GetEntryPoint() const { return assembly_.second; }

private:
    CompilerOptions options_ {};
    Assembly assembly_ {};
};
}  // namespace tcc

#endif