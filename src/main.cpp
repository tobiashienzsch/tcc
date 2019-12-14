#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/ast.hpp"
#include "tcc/vm/vm.hpp"

namespace
{
constexpr auto foo(int64_t x, int64_t y) -> int64_t
{
    auto const l1 = x + y;
    auto const l2 = y + 10;
    return l1 + l2;
};

static_assert(foo(10, 10) == 40);
static_assert(foo(4, 7) == 28);
}  // namespace

int main(int const, char const** const)
{
    using namespace tcc;

    auto const assembly = std::vector<tcc::Integer>{
        // .def foo: args=2, locals=1
        // l1 = x + y
        ByteCode::ICONST, 0,  // 0 local #1
        ByteCode::LOAD, -4,   // 2
        ByteCode::LOAD, -3,   // 4
        ByteCode::IADD,       // 6
        ByteCode::STORE, 1,   // 7

        // l2 = y + 10
        ByteCode::LOAD, -3,    // 9
        ByteCode::ICONST, 10,  // 11
        ByteCode::IADD,        // 13

        // l1 + l2
        ByteCode::LOAD, 1,  // 14
        ByteCode::IADD,     // 16
        ByteCode::RET,      // 17

        // .def main: args=0, locals=0
        // foo(10, 10)
        ByteCode::ICONST, 10,  // 18 <-- MAIN
        ByteCode::ICONST, 10,  // 20
        ByteCode::CALL, 0, 1,  // 22
        ByteCode::EXIT,        // 25
    };

    auto vm             = VirtualMachine(assembly, 18, 0, 100, true);
    auto const exitCode = vm.Cpu();

    return EXIT_SUCCESS;
}