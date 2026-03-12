cmake_minimum_required(VERSION 3.29)

project(C-Programming C)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

add_subdirectory("sources/CSVPrettyPrinter")
add_subdirectory("sources/AVLTree")
add_subdirectory("sources/Aerosoft")
add_subdirectory("sources/Graph")