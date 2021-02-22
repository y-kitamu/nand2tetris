cmake_minimum_required(VERSION 3.17.2)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(nand2tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -fPIC -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(LIBRARIES "" CACHE INTERNAL "")

set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${include_dir}")
add_subdirectory(src)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message("INCLUDE DIRS : ${dirs}")
