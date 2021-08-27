cmake_minimum_required(VERSION 3.17)
project(tractable)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party)
add_subdirectory(trbl)

add_executable(example main.cpp)
target_link_libraries(example tractable)

add_executable(trbl_benchmark benchmark.cpp)
target_link_libraries(trbl_benchmark tractable benchmark::benchmark_main)