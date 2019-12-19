project(tcc-spirit-parser)

add_executable(${PROJECT_NAME}
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

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Boost::program_options Boost::system fmt-header-only tcc::Library)