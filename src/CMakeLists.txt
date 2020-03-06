cmake_minimum_required(VERSION 3.0.0)
project(NLex VERSION 0.0.1)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

find_package(TBB REQUIRED tbb)

message(STATUS "Found TBB ${TBB_IMPORTED_TARGETS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_command(OUTPUT ../deser.inc
                   WORKING_DIRECTORY ..
                   BYPRODUCTS deser.inc.bc
                   COMMAND clang++ -g -c deser.inc.cc -emit-llvm
                   COMMAND xxd -i deser.inc.bc > deser.inc
)

add_custom_command(OUTPUT ../test_bc.h
                   WORKING_DIRECTORY ..
                   BYPRODUCTS test.bc
                   COMMAND clang -g -c rts.c -emit-llvm -o test.bc
                   COMMAND xxd -i test.bc > test_bc.h
)

add_custom_target(incs ALL
    DEPENDS deser.inc
    DEPENDS test_bc.h)

add_executable(nlex
    hmm.cc
    kaleid.cpp
    lexer.cc
    parser.cc)

add_dependencies(nlex incs)

llvm_config(nlex USE_SHARED core)

target_compile_options(nlex PRIVATE -Wno-narrowing -Wno-write-strings -pthread -g -ljansson) # TODO: Require jansson

target_link_libraries(nlex PRIVATE
    ${TBB_IMPORTED_TARGETS}
    pthread
)
