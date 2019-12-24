/**
 * @file vm.hpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */

#pragma once

#include "tcc/core.hpp"

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

namespace tcc
{
class VirtualMachine
{
public:
    explicit VirtualMachine(std::vector<int64_t> code,  //
                            uint64_t main,              //
                            uint64_t dataSize,          //
                            uint64_t stackSize = 100,   //
                            bool shouldTrace   = true   //
    );

    int64_t Cpu();

    void EnableTracing(bool const shouldTrace);

    void Reset(int64_t const entryPoint)
    {
        m_stackPointer       = -1;
        m_instructionPointer = entryPoint;
        m_framePointer       = 0;
    }

private:
    void disassemble(int64_t const opcode);
    void printStack();
    void printGlobalMemory();

private:
    int64_t m_stackPointer {-1};
    int64_t m_instructionPointer;
    int64_t m_framePointer {0};

    std::vector<int64_t> m_code;
    std::vector<int64_t> m_data;
    std::vector<int64_t> m_stack;

private:
    bool m_shouldTrace {true};
};
}  // namespace tcc