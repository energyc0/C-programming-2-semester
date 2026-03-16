add_library(CSVPrettyPrinterLib STATIC CSVPrettyPrinter.c)

add_executable(CSVPrettyPrinter CSVPrettyPrinterMain.c)

set_target_properties(CSVPrettyPrinterLib CSVPrettyPrinter PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(CSVPrettyPrinter PRIVATE CSVPrettyPrinterLib)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(CSVPrettyPrinterLib ${SANITIZER_LIBS})
endif()