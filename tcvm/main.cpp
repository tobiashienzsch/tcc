/**
 * @file main.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcvm/examples.hpp"
#include "tcvm/program_options.hpp"
#include "tsl/tsl.hpp"

auto main(int argc, char** argv) -> int
{

    auto cliArguments = po::variables_map {};
    if (!tcvm::ProgramOptions::Parse(argc, argv, cliArguments))
    {
        return EXIT_FAILURE;
    }

    auto arg = std::int64_t {};
    if (cliArguments.count("input") != 0U)
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

    // binary
    auto program = tcc::BinaryProgram {};
    tcc::BinaryFormat::ReadFromFile(path, program);
    auto const stackSize = 200;
    auto vm              = tcc::VirtualMachine(program.data, program.entryPoint, 0, stackSize, true);

    // factorial
    // auto const factorial = tcvm::CreateFactorialProgram(arg);
    // auto vm = tcc::VirtualMachine(factorial.data, factorial.entryPoint, 0, 1000, true);

    return vm.Cpu();
}