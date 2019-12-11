#pragma once

#include <cstdint>

#include <iostream>
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
    HALT   = 16,
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
    Instruction{"halt"},       //
};

class VirtualMachine
{
public:
    VirtualMachine(std::vector<int64_t> c, int64_t main, int64_t datasize)
        : m_instructionPointer(main), m_code(std::move(c)), m_data(datasize), m_stack(100)
    {
    }

    void Cpu()
    {

        while (static_cast<size_t>(m_instructionPointer) < m_code.size())
        {
            auto const opcode = m_code.at(m_instructionPointer);  // fetch instructions
            if (m_isTrace)
            {
                disassemble(opcode);
            }

            m_instructionPointer++;  // advance instruction pointer

            switch (opcode)
            {
                case ByteCode::ICONST:
                {
                    auto const val = m_code.at(m_instructionPointer);  // read operand from code
                    m_instructionPointer++;
                    m_stackPointer++;
                    m_stack[m_stackPointer] = val;  // push to stack
                    break;
                }
                case ByteCode::PRINT:
                {
                    auto const val = m_stack.at(m_stackPointer);
                    m_stackPointer--;
                    std::printf("%lld\n", val);
                    break;
                }
                case ByteCode::HALT: return;
            }
        }
    }

private:
    void disassemble(int64_t opcode)
    {
        auto const instruction = Instructions[opcode];
        std::printf("%04lld: ", m_instructionPointer);
        std::cout << ByteCode{opcode};
        if (instruction.numberOfOperands == 1) std::printf(" %lld", m_code.at(m_instructionPointer + 1));
        if (instruction.numberOfOperands == 2)
        {
            auto const firstOperand  = m_code.at(m_instructionPointer + 1);
            auto const secondOperand = m_code.at(m_instructionPointer + 2);
            std::printf(" %lld, %lld", firstOperand, secondOperand);
        }

        std::cout << '\n';
    }

private:
    int64_t m_stackPointer{-1};
    int64_t m_instructionPointer;
    int64_t m_framePointer;

    std::vector<int64_t> m_code;
    std::vector<int64_t> m_data;
    std::vector<int64_t> m_stack;

private:
    bool m_isTrace{true};
};
}  // namespace tcc