#pragma once

#include <cstdint>

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

class VirtualMachine
{
public:
    VirtualMachine(std::vector<int64_t> c, int64_t main, int64_t datasize)
        : ip(main), code(std::move(c)), data(datasize), stack(100)
    {
    }

    void Cpu()
    {

        while (ip < code.size())
        {
            auto const opcode = code.at(ip);  // fetch instructions
            ip++;                             // advance instruction pointer

            switch (opcode)
            {
                case ByteCode::ICONST:
                {
                    auto const val = code.at(ip);  // read operand from code
                    ip++;
                    sp++;
                    stack[sp] = val;  // push to stack
                    break;
                }
                case ByteCode::PRINT:
                {
                    auto const val = stack.at(sp);
                    sp--;
                    std::printf("%lld\n", val);
                    break;
                }
                case ByteCode::HALT: std::puts("HALT"); return;
            }
        }
    }

private:
    int64_t ip;  // instruction pointer
    int64_t sp{-1};
    int64_t fp;

    std::vector<int64_t> code;
    std::vector<int64_t> data;
    std::vector<int64_t> stack;
};
}  // namespace tcc