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
    std::ostream* OutStream = &std::cout;
    std::string Source      = "";
    std::string OutputName  = "";
    int OptLevel            = 0;
    bool PrintSource        = false;
    bool PrintAst           = false;
    bool PrintIR            = false;
    bool PrintAssembly      = false;
};

}  // namespace tcc