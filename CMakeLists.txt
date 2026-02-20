cmake_minimum_required(VERSION 3.29)

project(CSVPrettyPrinter C)

set(SOURCE_DIR "sources/CSVPrettyPrinter")
add_compile_options(-g)

add_library(CSVPrettyPrinterLib STATIC ${SOURCE_DIR}/CSVPrettyPrinter.c)

add_executable(CSVPrettyPrinter ${SOURCE_DIR}/CSVPrettyPrinterMain.c)

target_link_libraries(CSVPrettyPrinter PRIVATE CSVPrettyPrinterLib)