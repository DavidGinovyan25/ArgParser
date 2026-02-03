find_package(GTest REQUIRED)

add_executable(argparser_test
    argparser_test.cpp
)

target_link_libraries(argparser_test
    argparser           # ваша библиотека
    GTest::gtest_main   # ← ДОБАВЬТЕ ЭТО (предоставляет main)
    GTest::gtest        # ← ДОБАВЬТЕ ЭТО  
    pthread             # ← ДОБАВЬТЕ ЭТО (обязательно для gtest)
)