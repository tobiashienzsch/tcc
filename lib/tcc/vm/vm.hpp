#pragma once

#include <cstdint>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

namespace tcc
{
enum ByteCode : int64_t
{
    IADD   = 1,
    ISUB   = 2,
    IMUL   = 3,
    ILT    = 4,
    IEQ    = 5,
    BR     = 6,
    BRT    = 7,
    BRF    = 8,
    ICONST = 9,
    LOAD   = 10,
    GLOAD  = 11,
    STORE  = 12,
    GSTORE = 13,
    PRINT  = 14,
    POP    = 15,
    CALL   = 16,
    RET    = 17,
    HALT   = 18,
};

std::ostream& operator<<(std::ostream& out, ByteCode byteCode)
{
    switch (byteCode)
    {
        case ByteCode::IADD: return out << "IADD";
        case ByteCode::ISUB: return out << "ISUB";
        case ByteCode::IMUL: return out << "IMUL";
        case ByteCode::ILT: return out << "ILT";
        case ByteCode::IEQ: return out << "IEQ";
        case ByteCode::BR: return out << "BR";
        case ByteCode::BRT: return out << "BRT";
        case ByteCode::BRF: return out << "BRF";
        case ByteCode::ICONST: return out << "ICONST";
        case ByteCode::LOAD: return out << "LOAD";
        case ByteCode::GLOAD: return out << "GLOAD";
        case ByteCode::STORE: return out << "STORE";
        case ByteCode::GSTORE: return out << "GSTORE";
        case ByteCode::PRINT: return out << "PRINT";
        case ByteCode::POP: return out << "POP";
        case ByteCode::CALL: return out << "CALL";
        case ByteCode::RET: return out << "RET";
        case ByteCode::HALT: return out << "HALT";
    }
    return out;
}

struct Instruction
{
    std::string_view name;
    int8_t numberOfOperands;

    constexpr explicit Instruction(std::string_view _name) : name(_name), numberOfOperands(0) {}
    constexpr explicit Instruction(std::string_view _name, int8_t numOperands)
        : name(_name), numberOfOperands(numOperands)
    {
    }
};

constexpr Instruction Instructions[] = {
    Instruction{"invalid"},    //
    Instruction{"iadd"},       //
    Instruction{"isub"},       //
    Instruction{"imul"},       //
    Instruction{"ilt"},        //
    Instruction{"ieq"},        //
    Instruction{"br", 1},      //
    Instruction{"brt", 1},     //
    Instruction{"brf", 1},     //
    Instruction{"iconst", 1},  //
    Instruction{"load", 1},    //
    Instruction{"gload", 1},   //
    Instruction{"store", 1},   //
    Instruction{"gstore", 1},  //
    Instruction{"print"},      //
    Instruction{"pop"},        //
    Instruction{"call"},       //
    Instruction{"ret"},        //
    Instruction{"halt"},       //
};

class VirtualMachine
{
public:
    explicit VirtualMachine(std::vector<int64_t> code, int64_t const main, int64_t const dataSize,
                            int64_t const stackSize = 100)
        : m_instructionPointer(main), m_code(std::move(code)), m_data(dataSize), m_stack(stackSize)
    {
    }

    void Cpu()
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
                case ByteCode::BR:
                {
                    m_instructionPointer = m_code[m_instructionPointer++];
                    break;
                }

                case ByteCode::BRT:
                {
                    auto const addr = m_code[m_instructionPointer++];
                    if (m_stack[m_stackPointer--]) m_instructionPointer = addr;
                }

                case ByteCode::BRF:
                {
                    auto const addr = m_code[m_instructionPointer++];
                    if (!m_stack[m_stackPointer--]) m_instructionPointer = addr;
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
                    std::printf("%lld\n", val);
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
                    auto const numArgs        = m_code[m_instructionPointer++];  // how many args git pushed
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

                case ByteCode::HALT: return;
            }
        }
    }

    void EnableTracing(bool const shouldTrace) { m_shouldTrace = shouldTrace; }

private:
    void disassemble(int64_t const opcode)
    {
        auto const instruction = Instructions[opcode];
        std::printf("%04lld: ", m_instructionPointer);
        std::stringstream byteCodeStr{};
        byteCodeStr << ByteCode{opcode};
        std::printf("%s", byteCodeStr.str().c_str());
        if (instruction.numberOfOperands == 1) std::printf(" %lld", m_code.at(m_instructionPointer + 1));
        if (instruction.numberOfOperands == 2)
        {
            auto const firstOperand  = m_code.at(m_instructionPointer + 1);
            auto const secondOperand = m_code.at(m_instructionPointer + 2);
            std::printf(" %lld, %lld", firstOperand, secondOperand);
        }

        printGlobalMemory();
        printStack();
        std::puts("");
    }
    void printStack()
    {
        std::printf("\t STACK: [ ");
        for (auto i = 0; i <= m_stackPointer; i++)
        {
            auto const var = m_stack.at(i);
            std::printf("%lld ", var);
        }
        std::printf("]");
    }
    void printGlobalMemory()
    {
        std::printf("\t\t GLOBALS: [ ");
        for (auto const& global : m_data) std::printf("%04lld ", global);
        std::printf("]");
    }

private:
    int64_t m_stackPointer{-1};
    int64_t m_instructionPointer;
    int64_t m_framePointer;

    std::vector<int64_t> m_code;
    std::vector<int64_t> m_data;
    std::vector<int64_t> m_stack;

private:
    bool m_shouldTrace{true};
};
}  // namespace tcc