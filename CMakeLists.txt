cmake_minimum_required(VERSION 3.10)
project(ArgParserProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/ArgParser)
add_subdirectory(bin)

add_subdirectory(tests)
