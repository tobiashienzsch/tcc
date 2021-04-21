/**
 * @file vm.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcvm/vm/vm.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
VirtualMachine::VirtualMachine(std::vector<int64_t> code, uint64_t const main, uint64_t const dataSize,
                               uint64_t const stackSize, bool shouldTrace, std::ostream& out)
    : m_instructionPointer_(main)
    , m_code_(std::move(code))
    , m_data_(dataSize)
    , m_stack_(stackSize)
    , m_shouldTrace_(shouldTrace)
    , out_(out)
{
}

auto VirtualMachine::cpu() -> int64_t
{
    while (static_cast<size_t>(m_instructionPointer_) < m_code_.size())
    {

        // checks
        TCC_ASSERT((m_code_.size() > static_cast<std::size_t>(m_instructionPointer_)) && (m_instructionPointer_ >= 0),
                   "instruction pointer out of range at: {}", m_instructionPointer_);

        // fetch instructions
        auto const opcode = m_code_[m_instructionPointer_];
        if (m_shouldTrace_) { disassemble(opcode); }

        m_instructionPointer_++;  // advance instruction pointer

        switch (opcode)
        {
            case ByteCode::IADD:
            {
                auto const b                = m_stack_[m_stackPointer_--];  // 2nd operand
                auto const a                = m_stack_[m_stackPointer_--];  // 1st operand
                m_stack_[++m_stackPointer_] = a + b;                        // push result
                break;
            }

            case ByteCode::ISUB:
            {
                auto const b                = m_stack_[m_stackPointer_--];  // 2nd operand
                auto const a                = m_stack_[m_stackPointer_--];  // 1st operand
                m_stack_[++m_stackPointer_] = a - b;                        // push result
                break;
            }

            case ByteCode::IMUL:
            {
                auto const b                = m_stack_[m_stackPointer_--];  // 2nd operand
                auto const a                = m_stack_[m_stackPointer_--];  // 1st operand
                m_stack_[++m_stackPointer_] = a * b;                        // push result
                break;
            }

            case ByteCode::ILT:
            {
                auto const b                = m_stack_[m_stackPointer_--];  // 2nd operand
                auto const a                = m_stack_[m_stackPointer_--];  // 1st operand
                m_stack_[++m_stackPointer_] = a < b ? 1 : 0;                // push result
                break;
            }

            case ByteCode::IEQ:
            {
                auto const b                = m_stack_[m_stackPointer_--];  // 2nd operand
                auto const a                = m_stack_[m_stackPointer_--];  // 1st operand
                m_stack_[++m_stackPointer_] = a == b ? 1 : 0;               // push result
                break;
            }

            case ByteCode::BR:
            {
                m_instructionPointer_ = m_code_[m_instructionPointer_++];
                break;
            }

            case ByteCode::BRT:
            {
                auto const addr = m_code_[m_instructionPointer_++];
                if (m_stack_[m_stackPointer_--] != 0) { m_instructionPointer_ = addr; }
                break;
            }

            case ByteCode::BRF:
            {
                auto const addr = m_code_[m_instructionPointer_++];
                if (m_stack_[m_stackPointer_--] == 0) { m_instructionPointer_ = addr; }
                break;
            }

            case ByteCode::ICONST:
            {
                auto const val = m_code_[m_instructionPointer_];  // read operand from code
                m_instructionPointer_++;
                m_stackPointer_++;
                m_stack_[m_stackPointer_] = val;  // push to stack
                break;
            }

            case ByteCode::LOAD:
            {
                auto const offset           = m_code_[m_instructionPointer_++];
                m_stack_[++m_stackPointer_] = m_stack_[m_framePointer_ + offset];
                break;
            }

            case ByteCode::GLOAD:
            {
                auto const addr = m_code_[m_instructionPointer_];
                m_instructionPointer_++;
                auto const val = m_data_[addr];
                m_stackPointer_++;
                m_stack_[m_stackPointer_] = val;
                break;
            }

            case ByteCode::STORE:
            {
                auto const offset                  = m_code_[m_instructionPointer_++];
                m_stack_[m_framePointer_ + offset] = m_stack_[m_stackPointer_--];
                break;
            }

            case ByteCode::GSTORE:
            {
                auto const val = m_stack_[m_stackPointer_];
                m_stackPointer_--;
                auto const addr = m_code_[m_instructionPointer_];
                m_instructionPointer_++;
                m_data_[addr] = val;
                break;
            }

            case ByteCode::PRINT:
            {
                auto const val = m_stack_[m_stackPointer_];
                m_stackPointer_--;
                out_ << fmt::format("{}\n", val);
                break;
            }

            case ByteCode::POP:
            {
                --m_stackPointer_;
                break;
            }

            case ByteCode::CALL:
            {
                // expects all args on stack
                auto const addr    = m_code_[m_instructionPointer_++];  // addr of function
                auto const numArgs = m_code_[m_instructionPointer_++];  // how many args got
                                                                        // pushed
                m_stack_[++m_stackPointer_] = numArgs;                  // save num args
                m_stack_[++m_stackPointer_] = m_framePointer_;          // save frame pointer
                m_stack_[++m_stackPointer_] = m_instructionPointer_;    // save instruction pointer
                m_framePointer_             = m_stackPointer_;          // fp points to return addr on stack
                m_instructionPointer_       = addr;                     // jump to function
                break;
            }

            case ByteCode::RET:
            {
                auto const returnVal  = m_stack_[m_stackPointer_--];  // pop return value
                m_stackPointer_       = m_framePointer_;              // jump over locals to frame pointer
                m_instructionPointer_ = m_stack_[m_stackPointer_--];  // pop return addr
                m_framePointer_       = m_stack_[m_stackPointer_--];  // restore frame pointer
                auto const numArgs    = m_stack_[m_stackPointer_--];  // how many args to throw away
                m_stackPointer_ -= numArgs;                           // pop args
                m_stack_[++m_stackPointer_] = returnVal;              // leave result on stack
                break;
            }

            case ByteCode::EXIT:
            {
                auto const exitCode = m_stack_[m_stackPointer_];
                if (m_shouldTrace_) { out_ << fmt::format("---\nexit code: {}\n", exitCode); }
                return exitCode;
            }
            case ByteCode::HALT: return -1;
            default: TCC_ASSERT(false, "unknown instruction"); std::exit(EXIT_FAILURE);
        }
    }

    return -1;
}

void VirtualMachine::enableTracing(bool const shouldTrace) { m_shouldTrace_ = shouldTrace; }

void VirtualMachine::disassemble(int64_t const opcode)
{
    auto const instruction = gsl::at(Instructions, opcode);
    out_ << fmt::format("{:04}: {}", m_instructionPointer_, ByteCode::Type {opcode});
    if (instruction.numberOfOperands == 1) { out_ << fmt::format(" {}", m_code_.at(m_instructionPointer_ + 1)); }
    if (instruction.numberOfOperands == 2)
    {
        auto const firstOperand  = m_code_.at(m_instructionPointer_ + 1);
        auto const secondOperand = m_code_.at(m_instructionPointer_ + 2);
        out_ << fmt::format(" {}, {}", firstOperand, secondOperand);
    }

    printGlobalMemory();
    out_ << fmt::format("\t FRAME_PTR: {}", m_framePointer_);
    out_ << fmt::format("\t STACK_PTR: {}", m_stackPointer_);
    printStack();
    out_ << fmt::format("\n");
}
void VirtualMachine::printStack()
{
    out_ << fmt::format("\t STACK: [ ");

    for (auto i = 0; i <= m_stackPointer_; i++)
    {
        auto const var = m_stack_.at(i);
        out_ << fmt::format("{} ", var);
    }
    out_ << fmt::format("]");
}

void VirtualMachine::printGlobalMemory()
{
    out_ << fmt::format("\t\t GLOBALS: [ ");
    for (auto const& global : m_data_) { out_ << fmt::format("{:04} ", global); }
    out_ << fmt::format("]");
}

}  // namespace tcc