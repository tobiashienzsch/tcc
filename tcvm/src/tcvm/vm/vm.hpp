/**
 * @file vm.hpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */

#pragma once

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "tcvm/vm/byte_code.hpp"

namespace tcc
{
class VirtualMachine
{
public:
    explicit VirtualMachine(std::vector<Integer> code,  //
                            UInteger main,              //
                            UInteger dataSize,          //
                            UInteger stackSize = 100,   //
                            bool shouldTrace   = true   //
    );

    Integer Cpu();

    void EnableTracing(bool const shouldTrace);

    void Reset(Integer const entryPoint)
    {
        m_stackPointer       = -1;
        m_instructionPointer = entryPoint;
        m_framePointer       = 0;
    }

private:
    void disassemble(Integer const opcode);
    void printStack();
    void printGlobalMemory();

private:
    Integer m_stackPointer {-1};
    Integer m_instructionPointer;
    Integer m_framePointer {0};

    std::vector<Integer> m_code;
    std::vector<Integer> m_data;
    std::vector<Integer> m_stack;

private:
    bool m_shouldTrace {true};
};
}  // namespace tcc