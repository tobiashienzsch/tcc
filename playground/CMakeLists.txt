project(tcc-playground)

set( BOOST_INCLUDEDIR "C:\\Developer\\lib\\boost" CACHE PATH "Boost include path" )
set( BOOST_ROOT "C:\\Developer\\lib\\boost\\boost" CACHE PATH "Boost library path" )
set( Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )

find_package(Boost 1.69 REQUIRED)

add_executable(${PROJECT_NAME} 
    main_spirit_parser.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Boost::boost  
    tcc::Library
)