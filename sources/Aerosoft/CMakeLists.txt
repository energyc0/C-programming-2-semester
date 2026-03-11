add_executable(aerosoft aerosoft.c)

target_link_libraries(aerosoft PRIVATE AVLTree)

set_target_properties(aerosoft PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")