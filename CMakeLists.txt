cmake_minimum_required(VERSION 3.10)
project(ArgParserProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory globally
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src/ArgParser)
add_subdirectory(bin)

# Optional: add tests
add_subdirectory(tests)
