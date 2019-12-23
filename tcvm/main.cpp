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

    auto const path = std::string("test.tcb");

    // // write binary
    // {
    //     auto const program = tcvm::CreateFactorialProgram(arg);
    //     tcc::BinaryFormat::WriteToFile(path, program);
    // }

    // read binary
    auto program = tcc::BinaryProgram{};
    tcc::BinaryFormat::ReadFromFile(path, program);

    // execute
    auto vm = tcc::VirtualMachine(program.data, program.entryPoint, 0, 200, true);
    // auto vm             = tcc::VirtualMachine(tcvm::CreateFactorialProgram(arg), 22, 0, 1000, true);
    // auto vm             = tcc::VirtualMachine(tcvm::CreateAdditionProgram(20), 18, 0, 200, true);
    // auto vm             = tcc::VirtualMachine(tcvm::CreateCompiledProgram(), 0, 0, 200, true);

    fmt::print("---\nexit code: {}\n", vm.Cpu());

    return EXIT_SUCCESS;
}