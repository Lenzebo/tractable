cmake_minimum_required(VERSION 3.17)
project(tractable)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party)

add_library(tractable Backend.cpp Tracer.cpp backends/ChromeTracingBackend.cpp backends/ChromeTracingBackend.h Context.cpp Context.h backends/ConsoleBackend.cpp backends/ConsoleBackend.h backends/LockedThreadsafeBackend.cpp backends/LockedThreadsafeBackend.h backends/AsyncBackend.cpp backends/AsyncBackend.h)

add_executable(example main.cpp)
target_link_libraries(example tractable)

add_executable(trbl_benchmark benchmark.cpp)
target_link_libraries(trbl_benchmark tractable benchmark::benchmark_main)