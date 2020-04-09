#pragma once

#include <string>

namespace tcc
{
/**
 * @brief Compiler flags.
 */
struct CompilerOptions
{
    std::string Source     = "";
    std::string OutputName = "";
    int OptLevel           = 0;
    bool PrintSource       = false;
    bool PrintAst          = false;
    bool PrintIR           = true;
};

}  // namespace tcc