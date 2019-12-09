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

add_executable(GetStockListTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_stock_list.cpp
)

add_executable(ReadFromFileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_read_from_file.cpp
)

add_executable(WriteToFileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_write_to_file.cpp
)

add_executable(OptionsParserTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_options_parser.cpp
)