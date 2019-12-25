set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(threading_library INTERFACE)
add_library(tcc::Threads ALIAS threading_library)

target_link_libraries(threading_library INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
