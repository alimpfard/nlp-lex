include_directories(
	"../../modules/post/include")

file(GLOB SOURCES
    "source/libpredict.cc")
    
file(GLOB ESOURCES
    "source/main.cc")

add_library(predict STATIC ${SOURCES})
target_link_libraries(predict module_post)
set_target_properties(predict PROPERTIES
    OUTPUT_NAME "predict"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )

add_executable(tool_predict ${ESOURCES})
target_link_libraries(tool_predict module_post)
set_target_properties(tool_predict PROPERTIES
    OUTPUT_NAME "tool_predict"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )