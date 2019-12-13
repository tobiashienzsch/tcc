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
    explicit VirtualMachine(std::vector<int64_t> code,     //
                            int64_t const main,            //
                            int64_t const dataSize,        //
                            int64_t const stackSize = 100  //
    );

    int64_t Cpu();
    void EnableTracing(bool const shouldTrace);

private:
    void disassemble(int64_t const opcode);
    void printStack();
    void printGlobalMemory();

private:
    int64_t m_stackPointer{-1};
    int64_t m_instructionPointer;
    int64_t m_framePointer{0};

    std::vector<int64_t> m_code;
    std::vector<int64_t> m_data;
    std::vector<int64_t> m_stack;

private:
    bool m_shouldTrace{false};
};
}  // namespace tcc