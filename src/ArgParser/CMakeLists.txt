# Create a static library from ArgParser.cpp
add_library(argparser STATIC
    ArgParser.cpp
)

target_include_directories(argparser PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
