project(tcvm)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        tcc::Library
        tcc::CompilerOptions    
        Boost::boost 
        Boost::program_options 
        Boost::system 
        fmt
)