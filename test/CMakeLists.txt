cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

link_libraries(stockwatch)

add_executable(GetStockSymbolsTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_stock_symbols.cpp
)

add_executable(GetHistoricalDataTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_historical_data.cpp
)

add_executable(InputHistoricalDataTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_input_historical_data.cpp
)

add_executable(WriteFileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_write_file.cpp
)

add_executable(ReadFileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_read_file.cpp
)

add_executable(ParseCmdLineOptionsTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_get_cmd_line_options.cpp
)

add_executable(ExhibitsHTFTest
    ${CMAKE_CURRENT_SOURCE_DIR}/test_exhibits_high_tight_flag.cpp
)