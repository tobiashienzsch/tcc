project(tcc-lib)

if (WIN32)
  set(BOOST_INCLUDEDIR          "C:\\Developer\\lib\\boost"         CACHE PATH "Boost include path" )
  set(BOOST_ROOT                "C:\\Developer\\lib\\boost\\boost"  CACHE PATH "Boost library path" )
  set(Boost_NO_SYSTEM_PATHS     on                                  CACHE BOOL "Do not search system for Boost" )
else()
endif()

find_package(Boost 1.65 REQUIRED)

# Add library
set(TCC_LIB_SOURCES
    "${PROJECT_SOURCE_DIR}/tcc/compiler/ast.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler/ast.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler/lexer.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler/lexer.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler/parser.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler/parser_x3.hpp"

    "${PROJECT_SOURCE_DIR}/tcc/core/warnings.hpp"

    "${PROJECT_SOURCE_DIR}/tcc/vm/byte_code.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/byte_code.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/vm.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/vm.cpp"

    "${PROJECT_SOURCE_DIR}/tcc/tcc.hpp"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TCC_LIB_SOURCES})

add_library(${PROJECT_NAME} STATIC ${TCC_LIB_SOURCES})

add_library(tcc::Library ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} 
PUBLIC
    Boost::boost
PRIVATE
    tcc::CodeCoverage
)