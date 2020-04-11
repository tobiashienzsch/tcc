project(playground)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        tcc::tcc
        tcc::tcvm
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)
