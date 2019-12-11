#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"
#include "tcc/vm/vm.hpp"

int main()
{
    using namespace tcc;

    auto const instructions = std::vector<int64_t>{
        ByteCode::ICONST, 99,  //
        ByteCode::PRINT,       //
        ByteCode::HALT,        //
    };

    auto vm = VirtualMachine(instructions, 0, 0);
    vm.Cpu();

    return EXIT_SUCCESS;
}