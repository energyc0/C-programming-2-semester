add_library(AVLTree AVLTree.c)
target_include_directories(AVLTree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(AVLTreeTests AVLTreeTests.c AVLTreeDebug.c)

target_link_libraries(AVLTreeTests PRIVATE AVLTree)

message(STATUS "Included ${CMAKE_CURRENT_SOURCE_DIR} directory for AVL library.")