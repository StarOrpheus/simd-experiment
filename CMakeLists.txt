cmake_minimum_required(VERSION 3.24)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark CONFIG REQUIRED)
find_package(Sqlpp11 CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_link_libraries(main PRIVATE sqlpp11::sqlpp11)
