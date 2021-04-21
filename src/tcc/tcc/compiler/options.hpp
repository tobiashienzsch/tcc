#pragma once

#include <iostream>
#include <string>

namespace tcc
{
/**
 * @brief Compiler flags.
 */
struct CompilerOptions
{
    std::ostream* out = &std::cout;
    std::string source {};
    std::string outputName {};
    int optLevel {0};
    bool printSource {false};
    bool printAst {false};
    bool printIr {false};
    bool printAssembly {false};
};

}  // namespace tcc