project(hashmap_lib)

add_library(hashmap_lib INTERFACE)
target_include_directories(hashmap_lib INTERFACE ${CMAKE_SOURCE_DIR}/include)
