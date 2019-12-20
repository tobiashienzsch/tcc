#include "tcvm/vm/vm.hpp"

namespace tcc
{
VirtualMachine::VirtualMachine(std::vector<Integer> code, UInteger const main, UInteger const dataSize,
                               UInteger const stackSize, bool shouldTrace)
    : m_instructionPointer(main)
    , m_code(std::move(code))
    , m_data(dataSize)
    , m_stack(stackSize)
    , m_shouldTrace(shouldTrace)
{
}

Integer VirtualMachine::Cpu()
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
                if (m_stack[m_stackPointer--]) m_instructionPointer = addr;
                break;
            }

            case ByteCode::BRF:
            {
                auto const addr = m_code[m_instructionPointer++];
                if (!m_stack[m_stackPointer--]) m_instructionPointer = addr;
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
                std::printf("%d\n", val);
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

void VirtualMachine::disassemble(Integer const opcode)
{
    auto const instruction = Instructions[opcode];
    std::printf("%04d: ", m_instructionPointer);
    std::stringstream byteCodeStr{};
    byteCodeStr << ByteCode{opcode};
    std::printf("%s", byteCodeStr.str().c_str());
    if (instruction.numberOfOperands == 1) std::printf(" %d", m_code.at(m_instructionPointer + 1));
    if (instruction.numberOfOperands == 2)
    {
        auto const firstOperand  = m_code.at(m_instructionPointer + 1);
        auto const secondOperand = m_code.at(m_instructionPointer + 2);
        std::printf(" %d, %d", firstOperand, secondOperand);
    }

    printGlobalMemory();
    std::printf("\t STACK_PTR: %2d", m_stackPointer);
    printStack();
    std::puts("");
}
void VirtualMachine::printStack()
{
    std::printf("\t STACK: [ ");
    for (auto i = 0; i <= m_stackPointer; i++)
    {
        auto const var = m_stack.at(i);
        std::printf("%d ", var);
    }
    std::printf("]");
}
void VirtualMachine::printGlobalMemory()
{
    std::printf("\t\t GLOBALS: [ ");
    for (auto const& global : m_data) std::printf("%04d ", global);
    std::printf("]");
}

}  // namespace tcc