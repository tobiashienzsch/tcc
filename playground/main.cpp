#include "tcc/core/binary_format.hpp"

#include <cstdlib>

int main(int, char**)
{
    tcc::BinaryFormat::Write("test.tobi", {1, 1, 1, 1, 1});
    return EXIT_SUCCESS;
}