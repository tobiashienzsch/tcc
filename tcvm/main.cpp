#include "tcvm/program_options.hpp"

auto main(int argc, char** argv) -> int
{
    auto cliArguments = po::variables_map {};
    if (!tcvm::ProgramOptions::Parse(argc, argv, cliArguments)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}