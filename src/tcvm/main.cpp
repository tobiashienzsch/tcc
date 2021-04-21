/**
 * @file main.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcsl/tcsl.hpp"
#include "tcvm/examples.hpp"
#include "tcvm/program_options.hpp"

auto main(int argc, char** argv) -> int
{

    auto cliArguments = po::variables_map {};
    if (!tcvm::ProgramOptions::parse(argc, argv, cliArguments)) { return EXIT_FAILURE; }

    auto arg = std::int64_t {};
    if (cliArguments.count("input") != 0U)
    {
        arg = cliArguments["input"].as<std::int64_t>();
        fmt::print("{}\n", arg);
    }

    auto path = std::string {};
    if (cliArguments.count("file") != 0U) { path = cliArguments["file"].as<std::string>(); }

    // // write binary
    // {
    //     auto const program = tcvm::CreateFactorialProgram(arg);
    //     tcc::BinaryFormat::WriteToFile(path, program);
    // }

    // binary
    auto program = tcc::BinaryProgram {};
    tcc::BinaryFormat::readFromFile(path, program);
    auto const stackSize = 200;
    auto vm              = tcc::VirtualMachine(program.data, program.entryPoint, 0, stackSize, true);

    // factorial
    // auto const factorial = tcvm::CreateFactorialProgram(arg);
    // auto vm = tcc::VirtualMachine(factorial.data, factorial.entryPoint, 0,
    // 1000, true);

    vm.cpu();
    return EXIT_SUCCESS;
}