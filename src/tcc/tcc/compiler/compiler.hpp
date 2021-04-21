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

    [[nodiscard]] auto run() -> int
    {
        auto iter = options_.source.cbegin();
        auto end  = options_.source.cend();

        using IteratorType = std::string::const_iterator;
        auto errorHandler  = tcc::ErrorHandler<IteratorType> {iter, end, *options_.out};

        auto parser = tcc::Parser {errorHandler};
        if (!parser.parseSource(iter, end))
        {
            fmt::print(*options_.out, "Error while parsing!\n");
            return EXIT_FAILURE;
        }

        auto irGenerator = tcc::IRGenerator {errorHandler};  // IR Generator
        if (!irGenerator(parser.getAst()))
        {
            fmt::print(*options_.out, "Error while compiling!\n");
            return EXIT_FAILURE;
        }

        if (options_.optLevel > 0)
        {
            for (auto& func : irGenerator.currentPackage().functions)
            {
                auto optimizer = tcc::Optimizer(func);
                optimizer.optimize();
            }
        }

        if (options_.printSource) { fmt::print(*options_.out, "source:\n{}\n", options_.source); }

        if (options_.printIr) { fmt::print(*options_.out, "{}", irGenerator.currentPackage()); }

        auto const& package = irGenerator.currentPackage();
        assembly_           = tcc::AssemblyGenerator::build(package);

        if (options_.printAssembly) { tcc::ASMUtils::prettyPrint(*options_.out, assembly_); }

        if (!options_.outputName.empty())
        {
            auto binaryProgram = tcc::BinaryProgram {1, options_.outputName, assembly_.second, assembly_.first};
            if (!tcc::BinaryFormat::writeToFile(options_.outputName, binaryProgram))
            {
                fmt::print(*options_.out, "Error while writing binary!\n");
                return EXIT_FAILURE;
            }
        }

        return EXIT_SUCCESS;
    }

    [[nodiscard]] auto getAssembly() const -> std::vector<int64_t> const& { return assembly_.first; }
    [[nodiscard]] auto getEntryPoint() const -> int64_t { return assembly_.second; }

private:
    CompilerOptions options_ {};
    Assembly assembly_ {};
};
}  // namespace tcc

#endif