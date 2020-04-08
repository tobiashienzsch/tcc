/**
 * @file vm.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcvm/vm/vm.hpp"

namespace tcc
{
VirtualMachine::VirtualMachine(std::vector<int64_t> code, uint64_t const main, uint64_t const dataSize,
                               uint64_t const stackSize, bool shouldTrace)
    : m_instructionPointer(main)
    , m_code(std::move(code))
    , m_data(dataSize)
    , m_stack(stackSize)
    , m_shouldTrace(shouldTrace)
{
}

auto VirtualMachine::Cpu() -> int64_t
{
    while (static_cast<size_t>(m_instructionPointer) < m_code.size())
    {
        auto const opcode = m_code.at(m_instructionPointer);  // fetch instructions
        if (m_shouldTrace)
        {
            disassemble(opcode);
        }

        m_instructionPointer++;  // advance instruction pointer

        switch (opcode)
        {
            case ByteCode::IADD:
            {
                auto const b              = m_stack[m_stackPointer--];  // 2nd operand
                auto const a              = m_stack[m_stackPointer--];  // 1st operand
                m_stack[++m_stackPointer] = a + b;                      // push result
                break;
            }

            case ByteCode::ISUB:
            {
                auto const b              = m_stack[m_stackPointer--];  // 2nd operand
                auto const a              = m_stack[m_stackPointer--];  // 1st operand
                m_stack[++m_stackPointer] = a - b;                      // push result
                break;
            }

            case ByteCode::IMUL:
            {
                auto const b              = m_stack[m_stackPointer--];  // 2nd operand
                auto const a              = m_stack[m_stackPointer--];  // 1st operand
                m_stack[++m_stackPointer] = a * b;                      // push result
                break;
            }

            case ByteCode::ILT:
            {
                auto const b              = m_stack[m_stackPointer--];  // 2nd operand
                auto const a              = m_stack[m_stackPointer--];  // 1st operand
                m_stack[++m_stackPointer] = a < b ? 1 : 0;              // push result
                break;
            }

            case ByteCode::IEQ:
            {
                auto const b              = m_stack[m_stackPointer--];  // 2nd operand
                auto const a              = m_stack[m_stackPointer--];  // 1st operand
                m_stack[++m_stackPointer] = a == b ? 1 : 0;             // push result
                break;
            }

            case ByteCode::BR:
            {
                m_instructionPointer = m_code[m_instructionPointer++];
                break;
            }

            case ByteCode::BRT:
            {
                auto const addr = m_code[m_instructionPointer++];
                if (m_stack[m_stackPointer--] != 0)
                {
                    m_instructionPointer = addr;
                }
                break;
            }

            case ByteCode::BRF:
            {
                auto const addr = m_code[m_instructionPointer++];
                if (m_stack[m_stackPointer--] == 0)
                {
                    m_instructionPointer = addr;
                }
                break;
            }

            case ByteCode::ICONST:
            {
                auto const val = m_code.at(m_instructionPointer);  // read operand from code
                m_instructionPointer++;
                m_stackPointer++;
                m_stack[m_stackPointer] = val;  // push to stack
                break;
            }

            case ByteCode::LOAD:
            {
                auto const offset         = m_code[m_instructionPointer++];
                m_stack[++m_stackPointer] = m_stack[m_framePointer + offset];
                break;
            }

            case ByteCode::GLOAD:
            {
                auto const addr = m_code.at(m_instructionPointer);
                m_instructionPointer++;
                auto const val = m_data.at(addr);
                m_stackPointer++;
                m_stack.at(m_stackPointer) = val;
                break;
            }

            case ByteCode::STORE:
            {
                auto const offset                = m_code[m_instructionPointer++];
                m_stack[m_framePointer + offset] = m_stack[m_stackPointer--];
                break;
            }

            case ByteCode::GSTORE:
            {
                auto const val = m_stack.at(m_stackPointer);
                m_stackPointer--;
                auto const addr = m_code.at(m_instructionPointer);
                m_instructionPointer++;
                m_data.at(addr) = val;
                break;
            }

            case ByteCode::PRINT:
            {
                auto const val = m_stack.at(m_stackPointer);
                m_stackPointer--;
                fmt::print("{}\n", val);
                break;
            }

            case ByteCode::POP:
            {
                --m_stackPointer;
                break;
            }

            case ByteCode::CALL:
            {
                // expects all args on stack
                auto const addr           = m_code[m_instructionPointer++];  // addr of function
                auto const numArgs        = m_code[m_instructionPointer++];  // how many args got pushed
                m_stack[++m_stackPointer] = numArgs;                         // save num args
                m_stack[++m_stackPointer] = m_framePointer;                  // save frame pointer
                m_stack[++m_stackPointer] = m_instructionPointer;            // save instruction pointer
                m_framePointer            = m_stackPointer;                  // fp points to return addr on stack
                m_instructionPointer      = addr;                            // jump to function
                break;
            }

            case ByteCode::RET:
            {
                auto const returnVal = m_stack.at(m_stackPointer--);  // pop return value
                m_stackPointer       = m_framePointer;                // jump over locals to frame pointer
                m_instructionPointer = m_stack.at(m_stackPointer--);  // pop return addr
                m_framePointer       = m_stack.at(m_stackPointer--);  // restore frame pointer
                auto const numArgs   = m_stack.at(m_stackPointer--);  // how many args to throw away
                m_stackPointer -= numArgs;                            // pop args
                m_stack.at(++m_stackPointer) = returnVal;             // leave result on stack
                break;
            }

            case ByteCode::EXIT: return m_stack.at(m_stackPointer);
            case ByteCode::HALT: return -1;
        }
    }

    return -1;
}

void VirtualMachine::EnableTracing(bool const shouldTrace) { m_shouldTrace = shouldTrace; }

void VirtualMachine::disassemble(int64_t const opcode)
{
    auto const instruction = gsl::at(Instructions, opcode);
    fmt::printf("%04ld: ", m_instructionPointer);
    std::stringstream byteCodeStr {};
    byteCodeStr << ByteCode {opcode};
    fmt::print("{}", byteCodeStr.str().c_str());
    if (instruction.numberOfOperands == 1)
    {
        fmt::printf(" %ld", m_code.at(m_instructionPointer + 1));
    }
    if (instruction.numberOfOperands == 2)
    {
        auto const firstOperand  = m_code.at(m_instructionPointer + 1);
        auto const secondOperand = m_code.at(m_instructionPointer + 2);
        fmt::printf(" %ld, %ld", firstOperand, secondOperand);
    }

    printGlobalMemory();
    fmt::printf("\t STACK_PTR: %2ld", m_stackPointer);
    printStack();
    fmt::print("\n");
}
void VirtualMachine::printStack()
{
    fmt::print("\t STACK: [ ");

    for (auto i = 0; i <= m_stackPointer; i++)
    {
        auto const var = m_stack.at(i);
        fmt::printf("%ld ", var);
    }
    fmt::print("]");
}

void VirtualMachine::printGlobalMemory()
{
    fmt::print("\t\t GLOBALS: [ ");
    for (auto const& global : m_data)
    {
        fmt::printf("%04ld ", global);
    }
    fmt::print("]");
}

}  // namespace tcc