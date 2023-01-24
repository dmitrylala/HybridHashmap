project(hashmap_app)

set(EXE_SOURCES
	main.cpp
)

add_executable(hashmap ${EXE_SOURCES})

# target_link_libraries(hashmap PRIVATE sum_lib)
set_target_properties(hashmap PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#message(STATUS ${CMAKE_SOURCE_DIR} ${SOME_VAL})
#message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hashmap PUBLIC ${CMAKE_SOURCE_DIR}/include)

#SET(SOME_VAL2 dfjkdsfjk)
