cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

find_package(CURL REQUIRED)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(STOCKWATCH_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)

add_executable(StockWatch
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Stock.cpp
    ${SOURCE_DIR}/HistoricalData.cpp
    ${SOURCE_DIR}/StockSymbols.cpp
)

target_link_libraries(StockWatch PUBLIC
    -lcurl
    -lpthread
)

target_include_directories(StockWatch PUBLIC
    ${STOCKWATCH_INC_DIR}
    ${CURL_INCLUDE_DIR}
)