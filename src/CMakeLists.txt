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

add_executable(nlex
    kaleid.cpp
    lexer.cc
    parser.cc)

llvm_config(nlex USE_SHARED core)

target_compile_options(nlex PRIVATE -Wno-narrowing -Wno-write-strings -pthread)

target_link_libraries(nlex PRIVATE
    ${TBB_IMPORTED_TARGETS}
    pthread
)
