cmake_minimum_required(VERSION 3.24)
project(experimental-simd-benchmark)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark CONFIG REQUIRED)

add_compile_options(-Wall -Wextra -pedantic -march=native -DENABLE_AVX_INTRINSICS_TEST -DENABLE_SSE_INTRINSICS_TEST)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)
