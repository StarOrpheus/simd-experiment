cmake_minimum_required(VERSION 3.24)
project(experimental-simd-benchmark)

set(CMAKE_CXX_STANDARD 20)

set(BENCHMARK_ENABLE_TESTING 0)
set(BENCHMARK_ENABLE_GTEST_TESTS 0)
add_subdirectory(benchmark-1.7.1)

add_executable(gen_data generate_data.cpp)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)
