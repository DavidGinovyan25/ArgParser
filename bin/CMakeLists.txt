add_executable(argparser_main
    main.cpp
)

# Link your library to the executable
target_link_libraries(argparser_main
    argparser
)
