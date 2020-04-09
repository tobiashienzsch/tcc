#if !defined(TCC_CMD_PROHRAM_OPTIONS_HPP)
#define TCC_CMD_PROHRAM_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tcc/compiler/options.hpp"
#include "tsl/tsl.hpp"

namespace tcc
{
namespace po = boost::program_options;

/**
 * @brief Simple wrapper around boost program options.
 */
class ProgramOptions
{
public:
public:
    /**
     * @brief Defaulted constructor.
     */
    ProgramOptions() = default;

    /**
     * @brief Parses commandline arguments via argc & argv.
     *
     * @return First: Should exit program directly.
     * @return Second: Return code.
     */
    std::pair<bool, int> ParseArguments(int argc, char** argv)
    {
        try
        {
            // clang-format off
            po::options_description desc("Tobante's Crappy Compiler");
            auto options = desc.add_options();
            options("help,h", "produce this help message");
            options("input,i",          po::value<std::vector<std::string>>(),      "input source file");
            options("output,o",         po::value<std::string>(&flags_.OutputName), "output binary file");
            options("optimization,O",   po::value<int>(&flags_.OptLevel),           "optimization level 0-1");
            options("print-source",     po::bool_switch(&flags_.PrintSource),       "print source code");
            options("print-ast",        po::bool_switch(&flags_.PrintAst),          "print parsed ast");
            options("print-ir",         po::bool_switch(&flags_.PrintIR),           "print generated ir");
            options("print-asm",        po::bool_switch(&flags_.PrintAssembly),     "print generated asm");
            // clang-format on

            po::positional_options_description p;
            p.add("input", -1);

            po::store(po::command_line_parser(argc, argv)
                          .options(desc)
                          .positional(p)
                          .run(),
                      vm_);
            po::notify(vm_);

            if (vm_.count("help") != 0U)
            {
                fmt::print("{}\n", desc);
                return {true, EXIT_SUCCESS};
            }

            if (vm_.count("input") != 0U)
            {
                auto const paths = vm_["input"].as<std::vector<std::string>>();
                if (paths.size() > 1)
                {
                    fmt::print("Only one source file allowed currently.\n");
                    return {true, EXIT_FAILURE};
                }

                std::ifstream in(paths[0], std::ios_base::in);
                if (!in)
                {
                    fmt::print("Error: Could not open input file: {}\n",
                               paths[0]);
                    return {true, EXIT_FAILURE};
                }

                in.unsetf(std::ios::skipws);  // No white space skipping!
                flags_.Source = "";
                std::copy(std::istream_iterator<char>(in),
                          std::istream_iterator<char>(),
                          std::back_inserter(flags_.Source));
            }
        }
        catch (std::exception& e)
        {
            fmt::print("error: {}\n", e.what());
            return {true, EXIT_FAILURE};
        }
        catch (...)
        {
            fmt::print("Exception of unknown type!\n");
            return {true, EXIT_FAILURE};
        }

        return {false, EXIT_SUCCESS};
    }

    /**
     * @brief Returns the current compiler flags. They could change after
     * parsing argv.
     */
    [[nodiscard]] CompilerOptions const& GetCompilerOptions() const noexcept
    {
        return flags_;
    }

private:
    CompilerOptions flags_ {};
    po::variables_map vm_;
};
}  // namespace tcc
#endif