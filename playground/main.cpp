#include "tcc/ir/statement.hpp"
#include "tcsl/tcsl.hpp"

int main()
{
    auto package = tcc::IRPackage {};
    package.name = "test_package";
    package.functions.push_back(tcc::IRFunction {"foo"});
    fmt::print("{}\n", package);
    return EXIT_SUCCESS;
}