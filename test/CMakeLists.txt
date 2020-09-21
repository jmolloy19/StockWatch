find_package(GTest REQUIRED)

enable_testing()

add_executable(finnhub_tests
        main.cc
        finnhub_test.cc)
target_include_directories(finnhub_tests PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${GTEST_INCLUDE_DIRS})
target_link_libraries(finnhub_tests
        stockwatch
        ${GTEST_LIBRARIES})

add_test(FinnhubTests finnhub_tests)