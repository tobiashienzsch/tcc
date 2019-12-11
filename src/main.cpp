#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"
#include "tcc/vm/vm.hpp"

int main()
{
    using namespace tcc;

    // auto const instructions = std::vector<int64_t>{
    //     ByteCode::ICONST, 99,  //
    //     ByteCode::GSTORE, 0,   //
    //     ByteCode::GLOAD,  0,   //
    //     ByteCode::GSTORE, 1,   //
    //     ByteCode::GLOAD,  1,   //
    //     ByteCode::ICONST, 42,  //
    //     ByteCode::GSTORE, 3,   //
    //     ByteCode::GLOAD,  3,   //
    //     ByteCode::GLOAD,  1,   //
    //     ByteCode::PRINT,       //
    //     ByteCode::GLOAD,  3,   //
    //     ByteCode::PRINT,       //
    //     ByteCode::PRINT,       //
    //     ByteCode::PRINT,       //
    //     ByteCode::HALT,        //
    // };

    auto const instructions = std::vector<int64_t>{
        ByteCode::ICONST, 99,  //
        ByteCode::GSTORE, 0,   //
        ByteCode::GLOAD,  0,   //
        ByteCode::PRINT,       //
        ByteCode::HALT,        //
    };

    auto const factorial = std::vector<int64_t>{
        // .def fact: args=1, locals=0
        // if n < 2 return 1
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 2,  // 2
        ByteCode::ILT,        // 4
        ByteCode::BRF, 10,    // 5
        ByteCode::ICONST, 0,  // 7
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
        ByteCode::ICONST, 1,   // 22 <-- MAIN
        ByteCode::CALL, 0, 1,  // 24
        ByteCode::PRINT,       // 27
        ByteCode::HALT,        // 28
    };

    // auto vm = VirtualMachine(instructions, 0, 4);
    auto vm = VirtualMachine(factorial, 22, 0);
    vm.Cpu();

    return EXIT_SUCCESS;
}