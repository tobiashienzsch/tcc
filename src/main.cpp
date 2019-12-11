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

    auto const dataSize                = 4;
    auto const startInstructionPointer = 0;

    auto vm = VirtualMachine(instructions, startInstructionPointer, dataSize);
    vm.Cpu();

    return EXIT_SUCCESS;
}