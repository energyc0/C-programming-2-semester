add_library(graph graph.c)

add_executable(graphTest graphTest.c)

set_target_properties(graphTest graph PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(graphTest PRIVATE graph)