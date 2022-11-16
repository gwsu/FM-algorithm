cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(bw_partition)

# find_package(Torch REQUIRED PATHS externel/libtorch NO_DEFAULT_PATH)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
enable_testing()
add_subdirectory(lib)
add_subdirectory(src)
