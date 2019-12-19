project(tcc-core)

add_library(${PROJECT_NAME} INTERFACE )
add_library(tcc::Core ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})
