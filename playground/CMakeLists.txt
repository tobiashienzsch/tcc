project(tcc-playground)

if (WIN32)
  set(BOOST_INCLUDEDIR          "C:\\DeveBOOST_LIBRARYDIRloper\\lib\\boost"         CACHE PATH "Boost include path" )
  set(BOOST_LIBRARYDIR          "C:\\DeveBOOST_LIBRARYDIRloper\\lib\\boost"         CACHE PATH "Boost lib path" )
  set(BOOST_ROOT                "C:\\Developer\\lib\\boost\\boost"                  CACHE PATH "Boost library path" )
  set(Boost_NO_SYSTEM_PATHS     on                                                  CACHE BOOL "Do not search system for Boost" )
else()
endif()

find_package(Boost 1.70 REQUIRED)

# BOOST FUSION
add_executable(${PROJECT_NAME}_boost_fusion  main_boost_fusion.cpp)
target_compile_features(${PROJECT_NAME}_boost_fusion PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}_boost_fusion PRIVATE Boost::boost)

add_subdirectory(spirit_parser)