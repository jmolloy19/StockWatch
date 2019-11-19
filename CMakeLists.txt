cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_subdirectory(Source/Utility)

find_package(CURL)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(STOCKWATCH_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)

add_library(StockWatch STATIC
    ${SOURCE_DIR}/Stock.cpp
    ${SOURCE_DIR}/HistoricalData.cpp
    ${SOURCE_DIR}/StockSymbols.cpp
)

target_link_libraries(StockWatch PUBLIC
    -lcurl
    -lpthread
    Utility
)

target_include_directories(StockWatch PUBLIC
    ${STOCKWATCH_INC_DIR}
    ${CURL_INCLUDE_DIR}
)

add_executable(Analyze
    ${SOURCE_DIR}/Main.cpp
)

target_link_libraries(Analyze
    StockWatch
)

add_subdirectory(Test)