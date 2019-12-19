#include "tcvm/examples.hpp"
#include "tcvm/program_options.hpp"

auto main(int argc, char** argv) -> int
{
    auto cliArguments      = po::variables_map {};
    if (!tcvm::ProgramOptions::Parse(argc, argv, cliArguments)) return EXIT_FAILURE;

    // auto vm             = tcc::VirtualMachine(tcvm::createAdditionAssembly(20), 18, 0, 200, true);
    auto vm             = tcc::VirtualMachine(tcvm::createFactorialAssembly(5), 22, 0, 200, true);
    auto const exitCode = vm.Cpu();

    fmt::print("---\nexit code: {}\n", exitCode);

    return EXIT_SUCCESS;
}