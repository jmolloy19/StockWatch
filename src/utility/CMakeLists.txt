cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_library(utility STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/options.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/files.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/directory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/manual.cpp
)

target_include_directories(utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)