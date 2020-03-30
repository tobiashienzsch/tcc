#include "tcc-neo/parser/identifier.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
    auto const source = std::string {"test"};
    auto result       = std::string {};
    tcc::parser::x3::phrase_parse(std::begin(source),       //
                                  std::end(source),         //
                                  tcc::parser::identifier,  //
                                  tcc::parser::x3::space,   //
                                  result                    //
    );

    std::printf("%s\n", result.c_str());
    return EXIT_SUCCESS;
}