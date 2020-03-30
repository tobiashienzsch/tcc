#include "tcc-neo/parser/config.hpp"
#include "tcc-neo/parser/error_handler.hpp"
#include "tcc-neo/parser/package_def.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    auto const source = std::string {"package test;"};
    
    using tcc::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());
    std::printf("SOURCE:\n%s\n", source.c_str());

    // tcc::parser::PackageDeclaration_type ast;  // Our AST
    std::string ast;  // Our AST

    using boost::spirit::x3::with;
    using tcc::parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

    // Our parser
    // we pass our error handler to the parser so we can access
    // it later on in our on_error and on_sucess handlers
    auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[tcc::PackageDeclaration()];
    bool success      = tcc::parser::x3::phrase_parse(iter, end, parser, tcc::parser::x3::space, ast);

    if (success && iter == end)
    {
        std::printf("Parse success: %s\n", ast.c_str());
    }
    else
    {
        std::printf("Parse failed!\n");
    }

    return EXIT_SUCCESS;
}
