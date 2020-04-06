#if !defined(TCC_CMD_PROHRAM_OPTIONS_HPP)
#define TCC_CMD_PROHRAM_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tsl/tsl.hpp"

namespace tcc {
namespace po = boost::program_options;

class ProgramOptions {
 public:
  struct CompilerFlags {
    std::string Source = "";
    std::string OutputName = "";
    int OptLevel = 0;
    bool PrintSource = false;
    bool PrintAst = false;
    bool PrintIR = true;
  };

 public:
  ProgramOptions() = default;

  std::pair<bool, int> ParseArguments(int argc, char** argv) {
    try {
      // Declare a group of options that will be
      // allowed only on command line
      po::options_description desc("Tobante's Crappy Compiler");
      desc.add_options()                                                                  //
          ("help,h", "produce this help message")                                         //
          ("input,i", po::value<std::vector<std::string>>(), "input source file")         //
          ("output,o", po::value<std::string>(&flags_.OutputName), "output binary file")  //
          ("optimization,O", po::value<int>(&flags_.OptLevel), "optimization level 0-1")  //
          ("print-source", po::bool_switch(&flags_.PrintSource), "print source code")     //
          ("print-ast", po::bool_switch(&flags_.PrintAst), "print parsed ast")            //
          ("print-ir", po::bool_switch(&flags_.PrintIR), "print generated ir")            //
          ;

      po::positional_options_description p;
      p.add("input", -1);

      po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm_);
      po::notify(vm_);

      if (vm_.count("help")) {
        fmt::print("{}\n", desc);
        return {true, EXIT_SUCCESS};
      }

      if (vm_.count("input")) {
        auto const paths = vm_["input"].as<std::vector<std::string>>();
        if (paths.size() > 1) {
          fmt::print("Only one source file allowed currently.\n");
          return {true, EXIT_FAILURE};
        }

        std::ifstream in(paths[0], std::ios_base::in);
        if (!in) {
          std::cerr << "Error: Could not open input file: " << paths[0] << std::endl;
          return {true, EXIT_FAILURE};
        }

        in.unsetf(std::ios::skipws);  // No white space skipping!
        flags_.Source = "";
        std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(flags_.Source));
      }

    } catch (std::exception& e) {
      std::cerr << "error: " << e.what() << "\n";
      return {true, EXIT_FAILURE};
    } catch (...) {
      std::cerr << "Exception of unknown type!\n";
    }

    return {false, EXIT_SUCCESS};
  }

  CompilerFlags const& GetCompilerFlags() const noexcept { return flags_; }

 private:
  CompilerFlags flags_{};
  po::variables_map vm_;
};
}  // namespace tcc
#endif