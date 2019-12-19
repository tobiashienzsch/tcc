project(tcvm)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_features(${PROJECT_NAME} 
    PRIVATE 
        cxx_std_17
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        src
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Boost::boost 
        Boost::program_options 
        Boost::system 
        fmt-header-only 
        tcc::Library
)