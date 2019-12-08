cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

link_libraries(stockwatch)

add_executable(ProgressBarTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_progress_bar.cpp
)

add_executable(GetHistoricalDataTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_historical_data.cpp
)

add_executable(GetStockSymbolsTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_stock_symbols.cpp
)