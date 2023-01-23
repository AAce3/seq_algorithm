cmake_minimum_required(VERSION 3.24)
project(seq_algorithm C)

set(CMAKE_C_STANDARD 23)
find_package(OpenCL REQUIRED)
add_executable(seq_algorithm main.c algorithm.c algorithm.h)
target_link_libraries(seq_algorithm OpenCL::OpenCL)
