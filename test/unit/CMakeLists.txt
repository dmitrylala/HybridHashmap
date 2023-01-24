project(hashmap_test)

set(EXE_SOURCES
	hashmap.cpp
	bucket.cpp
)

add_executable(hashmap_test ${EXE_SOURCES})

target_link_libraries(hashmap_test PRIVATE hashmap_lib)
set_target_properties(hashmap_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
