#pragma once

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "byte_code.hpp"

namespace tcc
{
class VirtualMachine
{
public:
    explicit VirtualMachine(std::vector<Integer> code,     //
                            Integer const main,            //
                            Integer const dataSize,        //
                            Integer const stackSize = 100  //
    );

    Integer Cpu();
    void EnableTracing(bool const shouldTrace);

private:
    void disassemble(Integer const opcode);
    void printStack();
    void printGlobalMemory();

private:
    Integer m_stackPointer{-1};
    Integer m_instructionPointer;
    Integer m_framePointer{0};

    std::vector<Integer> m_code;
    std::vector<Integer> m_data;
    std::vector<Integer> m_stack;

private:
    bool m_shouldTrace{false};
};
}  // namespace tcc