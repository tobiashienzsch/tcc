#if !defined(TCC_CMD_PROHRAM_OPTIONS_HPP)
#define TCC_CMD_PROHRAM_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tcc/compiler/options.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
namespace po = boost::program_options;

/**
 * @brief Simple wrapper around boost program options.
 */
class ProgramOptions
{
public:
    /**
     * @brief Defaulted constructor.
     */
    explicit ProgramOptions(std::ostream& out = std::cout) : out_ {out} { }

    /**
     * @brief Parses commandline arguments via argc & argv.
     *
     * @return First: Should exit program directly.
     * @return Second: Return code.
     */
    auto parseArguments(int argc, char const* const* argv) -> std::pair<bool, int>
    {
        try
        {
            // clang-format off
            po::options_description desc("Tobante's Crappy Compiler");
            auto options = desc.add_options();
            options("help,h", "produce this help message");
            options("input,i",          po::value<std::vector<std::string>>(),      "input source file");
            options("output,o",         po::value<std::string>(&flags_.outputName), "output binary file");
            options("optimization,O",   po::value<int>(&flags_.optLevel),           "optimization level 0-1");
            options("print-source",     po::bool_switch(&flags_.printSource),       "print source code");
            options("print-ast",        po::bool_switch(&flags_.printAst),          "print parsed ast");
            options("print-ir",         po::bool_switch(&flags_.printIr),           "print generated ir");
            options("print-asm",        po::bool_switch(&flags_.printAssembly),     "print generated asm");
            // clang-format on

            po::positional_options_description p;
            p.add("input", -1);

            po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm_);
            po::notify(vm_);

            if (vm_.count("help") != 0U)
            {
                fmt::print(out_, "{}\n", desc);
                return {true, EXIT_SUCCESS};
            }

            if (vm_.count("input") != 0U)
            {
                auto const paths = vm_["input"].as<std::vector<std::string>>();
                if (paths.size() > 1)
                {
                    fmt::print(out_, "Only one source file allowed currently.\n");
                    return {true, EXIT_FAILURE};
                }

                tcc::File input {paths[0]};
                if (!input.exists())
                {
                    fmt::print(out_, "Could not open input file: {}\n", paths[0]);
                    return {true, EXIT_FAILURE};
                }

                flags_.source = input.loadAsString();
            }
        }
        catch (std::exception& e)
        {
            fmt::print(out_, "error: {}\n", e.what());
            return {true, EXIT_FAILURE};
        }
        catch (...)
        {
            fmt::print(out_, "Exception of unknown type!\n");
            return {true, EXIT_FAILURE};
        }

        return {false, EXIT_SUCCESS};
    }

    /**
     * @brief Returns the current compiler flags. They could change after
     * parsing argv.
     */
    [[nodiscard]] auto getCompilerOptions() const noexcept -> CompilerOptions const& { return flags_; }

private:
    CompilerOptions flags_ {};
    po::variables_map vm_;
    std::ostream& out_;
};
}  // namespace tcc
#endif