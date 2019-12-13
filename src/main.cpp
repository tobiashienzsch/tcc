#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/ast.hpp"
#include "tcc/vm/vm.hpp"

int main(int const argc, char const** const argv)
{
    int64_t fac = 1;
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
            std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
        }
    }

    using namespace tcc;

    auto const instructions = std::vector<int64_t>{
        ByteCode::ICONST, 99,  // 0
        ByteCode::GSTORE, 0,   // 2
        ByteCode::GLOAD,  0,   // 4
        ByteCode::ICONST, 2,   // 6
        ByteCode::ILT,         // 8
        ByteCode::BRT,    13,  // 9
        ByteCode::ICONST, 42,  // 11
        ByteCode::ICONST, 54,  // 13
        ByteCode::PRINT,       // 15
        ByteCode::HALT,        // 16

        ByteCode::ICONST, 143,  // 15
        ByteCode::PRINT,        // 17
        ByteCode::HALT,         // 18

    };

    auto const factorial = std::vector<int64_t>{
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

    tcc::BinaryExpression expression = {
        new tcc::ConstantExpression(10),
        new tcc::BinaryExpression(            //
            new tcc::ConstantExpression(5),   //
            new tcc::ConstantExpression(5),   //
            tcc::BinaryExpression::Type::Add  //
            ),
        tcc::BinaryExpression::Type::Add  //
    };

    auto assembly = expression.GetAssembly();
    assembly.push_back(tcc::ByteCode::PRINT);
    assembly.push_back(tcc::ByteCode::HALT);

    auto const stackSize = 200;

    auto vm = VirtualMachine(factorial, 22, 0, stackSize);
    // auto vm = VirtualMachine(instructions, 0, 4, stackSize);
    // auto vm = VirtualMachine(assembly, 0, 0, stackSize);
    auto const exitCode = vm.Cpu();

    return EXIT_SUCCESS;
}