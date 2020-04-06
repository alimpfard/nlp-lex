cmake_minimum_required(VERSION 3.0.0)
project(NLex VERSION 0.0.1)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(TBB tbb)

message(STATUS "Found TBB ${TBB_IMPORTED_TARGETS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/deser.inc
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   BYPRODUCTS ${CMAKE_SOURCE_DIR}/deser.inc.bc
                   COMMAND clang++ -g -c ${CMAKE_SOURCE_DIR}/deser.inc.cc -emit-llvm -o ${CMAKE_SOURCE_DIR}/deser.inc.bc
                   COMMAND xxd -i deser.inc.bc > ${CMAKE_SOURCE_DIR}/deser.inc
)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/test_bc.h
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   BYPRODUCTS ${CMAKE_SOURCE_DIR}/test.bc
                   COMMAND clang -g -c ${CMAKE_SOURCE_DIR}/rts.c -emit-llvm -o ${CMAKE_SOURCE_DIR}/test.bc
                   COMMAND xxd -i test.bc > ${CMAKE_SOURCE_DIR}/test_bc.h
)

add_custom_target(incs ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/deser.inc
    DEPENDS ${CMAKE_SOURCE_DIR}/test_bc.h)

add_executable(nlex
    hmm.cc
    kaleid.cpp
    lexer.cc
    parser.cc)

add_dependencies(nlex incs)

llvm_config(nlex USE_SHARED core)

target_include_directories(nlex PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
)

target_compile_options(nlex PRIVATE -Wno-narrowing -Wno-write-strings -pthread -g)

target_link_libraries(nlex PRIVATE
    ${TBB_IMPORTED_TARGETS}
    pthread
)
