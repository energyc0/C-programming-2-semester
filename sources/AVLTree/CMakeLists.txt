add_library(AVLTree AVLTree.c)

add_executable(AVLTreeTests AVLTreeTests.c)

target_link_libraries(AVLTreeTests PRIVATE AVLTree)