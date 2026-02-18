cmake_minimum_required(VERSION 3.29)

project(CSVPrettyPrinter C)

set(SOURCE_DIR "CSVPrettyPrinter")
add_compile_options(-g)

add_library(CSVPrettyPrinter STATIC ${SOURCE_DIR}/CSVPrettyPrinter.c)

add_executable(CSVPrettyPrinterTest ${SOURCE_DIR}/CSVPrettyPrinterTest.c)

target_link_libraries(CSVPrettyPrinterTest PRIVATE CSVPrettyPrinter)