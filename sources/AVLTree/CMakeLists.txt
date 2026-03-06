add_library(AVLTree AVLTree.c)

add_executable(AVLTreeTests AVLTreeTests.c AVLTreeDebug.c)

target_link_libraries(AVLTreeTests PRIVATE AVLTree)