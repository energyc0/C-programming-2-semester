add_library(AVLTree AVLTree.c)
target_include_directories(AVLTree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(AVLTreeTests AVLTreeTests.c AVLTreeDebug.c)

target_link_libraries(AVLTreeTests PRIVATE AVLTree)
set_target_properties(AVLTree AVLTreeTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

message(STATUS "Included ${CMAKE_CURRENT_SOURCE_DIR} directory for AVL library.")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(AVLTree ${SANITIZER_LIBS})
endif()