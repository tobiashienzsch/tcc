project(tcc)

set(tcc_lib_source
    src/ast.hpp
    src/ast_adapted.hpp
    src/common.hpp
    src/compiler.cpp
    src/compiler.hpp
    src/config.hpp
    src/error_handler.hpp
    src/expression_def.hpp
    src/expression.cpp
    src/expression.hpp
    src/statement_def.hpp
    src/statement.cpp
    src/statement.hpp
    src/vm.cpp
    src/vm.hpp

    main.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})

add_library(tcc_lib STATIC ${tcc_lib_source})
add_library(tcc::tcc ALIAS tcc_lib)

target_include_directories(tcc_lib 
    PUBLIC 
        src
)

target_link_libraries(tcc_lib 
    PUBLIC 
        Boost::boost 
        Boost::program_options 
        Boost::system 
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings

)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        tcc::tcc
        tcc::CompilerOptions
        tcc::CompilerWarnings
)

if(TCC_BUILD_TESTS)
    add_subdirectory(test)
endif()
