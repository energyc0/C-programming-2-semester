add_executable(graphStates graphStates.c)

target_link_libraries(graphStates PRIVATE graph)

set_target_properties(graphStates PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")