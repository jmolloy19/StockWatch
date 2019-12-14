cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

find_package(CURL)
set(STOCKWATCH_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_library(stockwatch STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/world_trading_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stockwatch.cpp
)

target_link_libraries(stockwatch PUBLIC
    -lpthread
    utility
)

target_include_directories(stockwatch PUBLIC
    ${STOCKWATCH_INC_DIR}
    ${CURL_INCLUDE_DIR}
)