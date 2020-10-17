#if !defined(TCC_COMPILER_COMPILER_HPP)
#define TCC_COMPILER_COMPILER_HPP

#include "tcc/asm/asm_generator.hpp"
#include "tcc/asm/asm_utils.hpp"

#include "tcc/compiler/options.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/parser.hpp"
#include "tcsl/tcsl.hpp"

#include <iostream>

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
        auto errorHandler  = tcc::ErrorHandler<IteratorType> {iter, end, *options_.Out};

        auto parser = tcc::Parser {errorHandler};
        if (!parser.ParseSource(iter, end))
        {
            fmt::print(*options_.Out, "Error while parsing!\n");
            return EXIT_FAILURE;
        }

        auto irGenerator = tcc::IRGenerator {errorHandler};  // IR Generator
        if (!irGenerator(parser.GetAst()))
        {
            fmt::print(*options_.Out, "Error while compiling!\n");
            return EXIT_FAILURE;
        }

        if (options_.OptLevel > 0)
        {
            for (auto& func : irGenerator.CurrentPackage().Functions)
            {
                auto optimizer = tcc::Optimizer(func);
                optimizer.Optimize();
            }
        }

        if (options_.PrintSource)
        {
            fmt::print(*options_.Out, "source:\n{}\n", options_.Source);
        }

        if (options_.PrintIR)
        {
            fmt::print(*options_.Out, "{}", irGenerator.CurrentPackage());
        }

        auto const& package = irGenerator.CurrentPackage();
        assembly_           = tcc::AssemblyGenerator::Build(package);

        if (options_.PrintAssembly)
        {
            tcc::ASMUtils::PrettyPrint(*options_.Out, assembly_);
        }

        if (!options_.OutputName.empty())
        {
            auto binaryProgram
                = tcc::BinaryProgram {1, options_.OutputName, assembly_.second, assembly_.first};
            if (!tcc::BinaryFormat::WriteToFile(options_.OutputName, binaryProgram))
            {
                fmt::print(*options_.Out, "Error while writing binary!\n");
                return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }

    [[nodiscard]] std::vector<int64_t> const& GetAssembly() const { return assembly_.first; }
    [[nodiscard]] int64_t GetEntryPoint() const { return assembly_.second; }

private:
    CompilerOptions options_ {};
    Assembly assembly_ {};
};
}  // namespace tcc

#endif