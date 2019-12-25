project(tcc-playground)

add_executable(asio_server asio_server.cpp)
set_target_properties(asio_server PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
target_link_libraries(asio_server 
    PRIVATE 
        Boost::boost 
        Boost::system 
        Boost::filesystem
        Boost::serialization 
        tcc::Core
        tcc::Threads 
        tcc::CompilerOptions 
        tcc::CompilerWarnings
)

add_executable(asio_client asio_client.cpp)
set_target_properties(asio_client PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
target_link_libraries(asio_client 
    PRIVATE 
        Boost::boost 
        Boost::system 
        Boost::filesystem
        Boost::serialization 
        tcc::Core
        tcc::Threads 
        tcc::CompilerOptions 
        tcc::CompilerWarnings
)