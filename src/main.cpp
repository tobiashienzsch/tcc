#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/ast.hpp"
#include "tcc/vm/vm.hpp"

int main(int const argc, char const** const argv)
{
    tcc::Integer fac = 1;
    if (argc == 2)
    {
        try
        {
            int i = std::stoi(argv[1]);
            fac   = i;
            std::cout << i << '\n';
        }
        catch (std::invalid_argument const&)
        {
            std::cout << "Bad input: std::invalid_argument thrown" << '\n';
        }
        catch (std::out_of_range const&)
        {
            std::cout << "tcc::Integer overflow: std::out_of_range thrown" << '\n';
        }
    }

    using namespace tcc;

    auto const factorial = std::vector<tcc::Integer>{
        // .def fact: args=1, locals=0
        // if n < 2 return 1
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 2,  // 2
        ByteCode::ILT,        // 4
        ByteCode::BRF, 10,    // 5
        ByteCode::ICONST, 1,  // 7
        ByteCode::RET,        // 9
        // return n * fact(n-1)
        ByteCode::LOAD, -3,    // 10
        ByteCode::LOAD, -3,    // 12
        ByteCode::ICONST, 1,   // 14
        ByteCode::ISUB,        // 16
        ByteCode::CALL, 0, 1,  // 17
        ByteCode::IMUL,        // 20
        ByteCode::RET,         // 21

        // .def main: args=0, locals=0
        ByteCode::ICONST, fac,  // 22 <-- MAIN
        ByteCode::CALL, 0, 1,   // 24
        ByteCode::EXIT,         // 27
    };

    auto const assembly = std::vector<tcc::Integer>{
        ByteCode::ICONST, 10,  // 0 <-- MAIN
        ByteCode::GSTORE, 0,   // 2
        ByteCode::ICONST, 1,   //
        ByteCode::ICONST, 2,   //
        ByteCode::ICONST, 3,   //
        ByteCode::GLOAD,  0,   //
        ByteCode::EXIT,        //
    };

    auto const entryPoint       = 22;
    auto const globalMemorySize = 1;
    auto const stackSize        = 200;

    // auto vm = VirtualMachine(factorial, entryPoint, globalMemorySize, stackSize);
    auto vm = VirtualMachine(assembly, 0, globalMemorySize, stackSize);
    vm.EnableTracing(true);
    auto const exitCode = vm.Cpu();

    return EXIT_SUCCESS;
}