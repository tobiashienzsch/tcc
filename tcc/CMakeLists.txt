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

add_executable(${PROJECT_NAME} ${tcc_lib_source})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        src
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        tcc::Core
        
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings

)