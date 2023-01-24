cmake_minimum_required(VERSION 3.16)

project(hybrid_hashmap)

message(STATUS ${CMAKE_BINARY_DIR})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(lib/)
add_subdirectory(bin/)
add_subdirectory(test/unit)
