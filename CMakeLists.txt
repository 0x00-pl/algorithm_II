cmake_minimum_required(VERSION 2.6)
project(algorithm_ii)

SET(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(algorithm_ii main.cpp)

install(TARGETS algorithm_ii RUNTIME DESTINATION bin)

add_subdirectory(src)