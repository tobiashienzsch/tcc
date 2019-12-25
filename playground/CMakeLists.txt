project(tcc-playground)

add_executable(${PROJECT_NAME} main.cpp grammar.cpp grammar.hpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Boost::boost 
        tcc::Core 
        tcc::CompilerOptions 
        tcc::CompilerWarnings
)