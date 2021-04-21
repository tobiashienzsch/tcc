/**
 * @file vm.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "tcsl/tcsl.hpp"

namespace tcc
{
class VirtualMachine
{
public:
    explicit VirtualMachine(std::vector<int64_t> code,  //
                            uint64_t main,              //
                            uint64_t dataSize,          //
                            uint64_t stackSize,         //
                            bool shouldTrace  = true,   //
                            std::ostream& out = std::cout);

    auto cpu() -> int64_t;

    void enableTracing(bool shouldTrace);

    void reset(int64_t const entryPoint)
    {
        m_stackPointer_       = -1;
        m_instructionPointer_ = entryPoint;
        m_framePointer_       = 0;
    }

private:
    void disassemble(int64_t opcode);
    void printStack();
    void printGlobalMemory();

    int64_t m_stackPointer_ {-1};
    int64_t m_instructionPointer_;
    int64_t m_framePointer_ {0};

    std::vector<int64_t> m_code_;
    std::vector<int64_t> m_data_;
    std::vector<int64_t> m_stack_;

    bool m_shouldTrace_ {true};
    std::ostream& out_;
};
}  // namespace tcc