/**
 * @file main.cpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */

#include "tcc/core/binary_format.hpp"

#include "tcvm/examples.hpp"
#include "tcvm/program_options.hpp"

auto main(int argc, char** argv) -> int
{
    auto cliArguments = po::variables_map{};
    if (!tcvm::ProgramOptions::Parse(argc, argv, cliArguments)) return EXIT_FAILURE;

    auto arg = std::int64_t{};
    if (cliArguments.count("input"))
    {
        arg = cliArguments["input"].as<std::int64_t>();
        fmt::print("{}\n", arg);
    }

    auto const path          = std::string("test.tcb");
    auto const writeAssembly = tcvm::CreateFactorialAssembly(arg);
    auto const writeProgram  = tcc::BinaryProgram{1, "project1", 0, writeAssembly};
    tcc::BinaryFormat::WriteToFile(path, writeProgram);

    auto program = tcc::BinaryProgram{};
    tcc::BinaryFormat::ReadFromFile(path, program);

    // auto vm             = tcc::VirtualMachine(tcvm::CreateFactorialAssembly(arg), 22, 0, 1000, true);
    // auto vm             = tcc::VirtualMachine(tcvm::CreateAdditionAssembly(20), 18, 0, 200, true);
    // auto vm             = tcc::VirtualMachine(tcvm::CreateCompiledAssembly(), 0, 0, 200, true);

    auto const entryPoint = 22;
    auto vm               = tcc::VirtualMachine(program.data, program.entryPoint, 0, 200, true);
    auto const exitCode   = vm.Cpu();
    fmt::print("---\nexit code: {}\n", exitCode);

    return EXIT_SUCCESS;
}